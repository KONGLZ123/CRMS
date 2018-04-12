#ifndef IPC_SHM_H
#define IPC_SHM_H

#include <windows.h>

class native_shm_interface
{
public:
    virtual ~native_shm_interface() {}
    virtual int open(const std::string &name) = 0;
    virtual int close() = 0;
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

private:
    native_shm_interface *shmOp;
};

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
    return 0;
}

template<uint32_t BLOCK_NUM, uint32_t BLOCK_SIZE>
int shm_ipc<BLOCK_NUM, BLOCK_SIZE>::close_channel(int id, CHANNEL_OPEN_MODE mode)
{
    return 0;
}

#endif


