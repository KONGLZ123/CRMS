#ifndef IPC_SHM_H
#define IPC_SHM_H

#include <windows.h>
#include <vector>

#define AtomicTestAndSet(x, y)  InterlockedExchange64((LONG64*)(x), (y))
#define AtomicAddAndFetch(x, y) (InterlockedExchangeAdd64((LONG64*)(x), (y)))
#define AtomicFetchAndAdd(x, y) (InterlockedExchangeAdd64((LONG64*)(x), (y)))
#define AtomicGet(x)            AtomicFetchAndAdd(x, 0)
#define YIELD()                 Sleep(1)
#define ARRAY_ANY_SIZE          1

class native_shm_interface
{
public:
    virtual ~native_shm_interface() {}
    virtual int open(const std::string &name) = 0;
    virtual int close() = 0;
    virtual void *address() = 0;
};

class win32_shm : public native_shm_interface
{
public:
    win32_shm()
        : creator_file_handler(NULL)
        , open_file_handler(NULL)
        , region_size(0)
        , addr(NULL)
    {}
    ~win32_shm()
    {
        close();
    }

    virtual int open(const std::string &name) override
    {
        open_file_handler = ::OpenFileMappingA(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, name.c_str());
        if (open_file_handler == NULL)
            return -1;

        addr = ::MapViewOfFile(open_file_handler, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
        if (addr == NULL) {
            close();
            return -1;
        }

        MEMORY_BASIC_INFORMATION info = { 0 };
        SIZE_T ret = ::VirtualQuery(addr, &info, sizeof info);
        if (ret == 0) {
            close();
            return -1;
        }
        region_size = info.RegionSize;

        return 0;
    }

    virtual int close() override
    {
        if (addr) {
            UnmapViewOfFile(&addr);
            addr = NULL;
            region_size = 0;
        }
        if (!open_file_handler) {
            ::CloseHandle(open_file_handler);
            open_file_handler = NULL;
        }
        return 0;
    }

    void *address() override
    {
        return addr;
    }

private:
    HANDLE creator_file_handler;
    HANDLE open_file_handler;
    int64_t region_size;
    PVOID addr;
};

enum CHANNEL_OPEN_MODE
{
    CHANNEL_READ = 1,
    CHANNEL_WRITE
};

template<uint32_t BLOCK_NUM, uint32_t BLOCK_SIZE>
class shm_ipc
{
public:
    shm_ipc();
    virtual ~shm_ipc();

    int open(const std::string &name);
    int close();
    int open_channel(int id, CHANNEL_OPEN_MODE mode);
    int close_channel(int id, CHANNEL_OPEN_MODE mode);
    int32_t write(int fd, const std::vector<std::pair<char*, int32_t>>& bufs);
    int32_t read(int fd, void *buf, int32_t size);

private:
    native_shm_interface *shmOp;
};

template<uint32_t BLOCK_SIZE>
struct shm_block
{
    int64_t lock;
    int64_t data_available;
    int64_t amount;
    int64_t channel_offset;
    unsigned char buf[BLOCK_SIZE];
};

template<uint32_t BLOCK_NUM, uint32_t BLOCK_SIZE>
struct shm_channel
{
    int64_t used;
    int64_t read_opened;
    int64_t write_opened;
    uint64_t writen_count;
    uint64_t read_count;
    int64_t header_offset;
    // multiple buffers so we can pipe line
    // read and write on the same channel
    shm_block<BLOCK_SIZE> block[BLOCK_NUM];
};

template<uint32_t BLOCK_NUM, uint32_t BLOCK_SIZE>
struct shm_header
{
    int64_t size;
    int64_t channel_count;
    shm_channel<BLOCK_NUM, BLOCK_SIZE> channels[ARRAY_ANY_SIZE];
};


template<uint32_t BLOCK_NUM, uint32_t BLOCK_SIZE>
static shm_channel<BLOCK_NUM, BLOCK_SIZE> *shm_block_get_channel(shm_block<BLOCK_SIZE> *block)
{
    unsigned char *addr = (unsigned char *)block;
    shm_channel<BLOCK_NUM, BLOCK_SIZE> *channel = (shm_channel<BLOCK_NUM, BLOCK_SIZE> *)(addr - block->channel_offset);
    return channel;
}

template<uint32_t BLOCK_NUM, uint32_t BLOCK_SIZE>
static shm_header<BLOCK_NUM, BLOCK_SIZE> *shm_channel_get_header(shm_channel<BLOCK_NUM, BLOCK_SIZE> *channel)
{
    unsigned char *addr = (unsigned char *)channel;
    shm_header<BLOCK_NUM, BLOCK_SIZE> *header = (shm_header<BLOCK_NUM, BLOCK_SIZE> *)(addr - channel->header_offset);
    return header;
}

/**
* helper functions for shm_block
*/
template<uint32_t BLOCK_NUM, uint32_t BLOCK_SIZE>
static int shm_block_lock(shm_block<BLOCK_SIZE> *block)
{
    int retry_count = 0;

    // == 1: it is previously 1 and we set it to 1, means state not changed
    // == 0: it is previously 0 and we set it to 1, means we get the lock
    while (AtomicTestAndSet(&block->lock, 1) == 1) {
        // if channel not available during wait, return error
        if (AtomicGet(&(shm_block_get_channel<BLOCK_NUM, BLOCK_SIZE>(block)->used)) == 0)
            return -1;

        // loop locally for some time, so we can avoid sleep system call
        // we assume the lock test fails quite little because use have
        // more than one buffers for one channel, so we can pipe line the
        // read and write operation
        //retry_count++;
        //if (retry_count > 20) {
        //   retry_count = 0;
        YIELD();
        //}
    }

    // check again after lock acquired
    // if channel not available during wait, return error
    if (AtomicGet(&(shm_block_get_channel<BLOCK_NUM, BLOCK_SIZE>(block)->used)) == 0) {
        AtomicTestAndSet(&block->lock, 0);
        return -1;
    }

    return 0;
}

template<uint32_t BLOCK_SIZE>
static void shm_block_unblock(shm_block<BLOCK_SIZE> *block)
{
    AtomicTestAndSet(&block->lock, 0);
}

template<uint32_t BLOCK_NUM, uint32_t BLOCK_SIZE>
static int shm_block_wait_for_data_state(shm_block<BLOCK_SIZE> *block, int64_t state)
{
    int retry_count = 0;
    shm_channel<BLOCK_NUM, BLOCK_SIZE> *channel = shm_block_get_channel<BLOCK_NUM, BLOCK_SIZE>(block);
    shm_header<BLOCK_NUM, BLOCK_SIZE> *header = shm_channel_get_header<BLOCK_NUM, BLOCK_SIZE>(channel);

    while (1) {
        if (shm_block_lock<BLOCK_NUM, BLOCK_SIZE>(block) != 0)
            return -1;

        // check read opened on read operation
        // this can make read operation return immediately after
        // closing read end of the channel
        if (state == 1 && AtomicGet(&channel->read_opened) == 0)
            return -1;

        // check write opened on write operation
        // this can make write operation return immediately after
        // closing write end of the channel
        if (state == 0 && AtomicGet(&channel->write_opened) == 0)
            return -1;

        if (AtomicGet(&block->data_available) == state) {
            // state is what we expected, we done
            // do not release lock because we are going to read/write data
            break;
        }
        else {
            // continue loop
            shm_block_unblock<BLOCK_SIZE>(block);
            retry_count++;
            if (retry_count > 20) {
                retry_count = 0;
                YIELD();
            }
        }
    }
    return 0;
}

template<uint32_t BLOCK_NUM, uint32_t BLOCK_SIZE>
shm_ipc<BLOCK_NUM, BLOCK_SIZE>::shm_ipc()
{
    shmOp = new win32_shm();
}

template<uint32_t BLOCK_NUM, uint32_t BLOCK_SIZE>
shm_ipc<BLOCK_NUM, BLOCK_SIZE>::~shm_ipc()
{
    delete shmOp;
}

template<uint32_t BLOCK_NUM, uint32_t BLOCK_SIZE>
int shm_ipc<BLOCK_NUM, BLOCK_SIZE>::open(const std::string & name)
{
    return shmOp->open(name);
}

template<uint32_t BLOCK_NUM, uint32_t BLOCK_SIZE>
int shm_ipc<BLOCK_NUM, BLOCK_SIZE>::close()
{
    return shmOp->close();
}

template<uint32_t BLOCK_NUM, uint32_t BLOCK_SIZE>
int shm_ipc<BLOCK_NUM, BLOCK_SIZE>::open_channel(int id, CHANNEL_OPEN_MODE mode)
{
    shm_header<BLOCK_NUM, BLOCK_SIZE> *header = (shm_header<BLOCK_NUM, BLOCK_SIZE> *)shmOp->address();

    if (!header)
        return -1;

    if (id < 0 || id > header->channel_count)
        return -1;

    if (AtomicGet(&header->channels[id].used) &&
        AtomicGet(&header->channels[id].read_opened) &&
        AtomicGet(&header->channels[id].write_opened))
        return -1;

    if ((mode == CHANNEL_READ) &&
        AtomicGet(&header->channels[id].read_opened))
        return -1;

    if ((mode == CHANNEL_WRITE) &&
        AtomicGet(&header->channels[id].write_opened))
        return -1;

    if (mode == CHANNEL_READ) {
        AtomicTestAndSet(&header->channels[id].read_opened, 1);
    }
    else if (mode == CHANNEL_WRITE) {
        AtomicTestAndSet(&header->channels[id].write_opened, 1);
    }
    else {
        return -1;
    }

    AtomicTestAndSet(&header->channels[id].used, 1);
    return 0;
}

template<uint32_t BLOCK_NUM, uint32_t BLOCK_SIZE>
int shm_ipc<BLOCK_NUM, BLOCK_SIZE>::close_channel(int id, CHANNEL_OPEN_MODE mode)
{
    shm_header<BLOCK_NUM, BLOCK_SIZE> *header = (shm_header<BLOCK_NUM, BLOCK_SIZE> *)shmOp->address();
    if (!header)
        return -1;

    if (id < 0 || id > header->channel_count)
        return -1;

    if ((mode == CHANNEL_READ) &&
        !AtomicGet(&header->channels[id].read_opened))
        return -1;

    if ((mode == CHANNEL_WRITE) &&
        !AtomicGet(&header->channels[id].write_opened))
        return -1;

    if (mode == CHANNEL_READ)
        AtomicTestAndSet(&header->channels[id].read_opened, 0);
    else if (mode == CHANNEL_WRITE)
        AtomicTestAndSet(&header->channels[id].write_opened, 0);
    else
        return -1;

    if (AtomicGet(&header->channels[id].read_opened) == 0 &&
        AtomicGet(&header->channels[id].write_opened) == 0)
        AtomicTestAndSet(&header->channels[id].used, 0);

    return 0;
}

template<uint32_t BLOCK_NUM, uint32_t BLOCK_SIZE>
inline int32_t shm_ipc<BLOCK_NUM, BLOCK_SIZE>::write(int fd, const std::vector<std::pair<char*, int32_t>>& bufs)
{
    return int32_t();
}

template<uint32_t BLOCK_NUM, uint32_t BLOCK_SIZE>
inline int32_t shm_ipc<BLOCK_NUM, BLOCK_SIZE>::read(int fd, void * buf, int32_t size)
{
    shm_header<BLOCK_NUM, BLOCK_SIZE> *header = (shm_header<BLOCK_NUM, BLOCK_SIZE> *)shmOp->address();
    if (fd < 0 || fd > header->channel_count)
        return -1;

    shm_channel<BLOCK_NUM, BLOCK_SIZE> *channel = &header->channels[fd];
    if (AtomicGet(&channel->used) != 1 ||
        AtomicGet(&channel->read_opened) != 1)
        return -1;

    int64_t read_idx = AtomicFetchAndAdd(&channel->read_count, 1);
    read_idx %= BLOCK_NUM;

    shm_block<BLOCK_SIZE> *block = &channel->block[read_idx];
    int32_t read_size = 0;

    // wait on block data wrote by remote (data_available == 1)
    // we hold the block lock after this function returns success
    int ret = shm_block_wait_for_data_state<BLOCK_NUM, BLOCK_SIZE>(block, 1);
    if (ret != 0) {
        AtomicFetchAndAdd(&channel->read_count, -1);
        return -1;
    }

    read_size = (int32_t)((size > block->amount) ? block->amount : size);
    memcpy(buf, block->buf, read_size);
    block->amount -= read_size;
    if (block->amount > 0)
        AtomicFetchAndAdd(&channel->read_count, -1);
    else
        block->data_available = 0;
    shm_block_unblock<BLOCK_SIZE>(block);

    return read_size;
}

#endif


