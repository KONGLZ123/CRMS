#include "video_source_ipc.h"
#include "ipc_shm.h"

#define VIDEO_SOURCE_BLOCK_NUM      10
#define VIDEO_SOURCE_BLOCK_SIZE     1024

struct VideoSourceIpcMsgHeader
{
    AgoraIpcMsg msg;
    uint32_t len;
};

class AgoraVideoSourceIpcImpl : public IAgoraIpc
{
public:
    AgoraVideoSourceIpcImpl(AgoraIpcListener *listener);
    ~AgoraVideoSourceIpcImpl();

    virtual bool initialize(const std::string &id) override;
    virtual bool listen() override;
    virtual bool connect() override;
    virtual bool disconnect() override;
    virtual bool run() override;
    virtual bool sendMessage(AgoraIpcMsg msg, char *payload, unsigned int len) override;

private:
    shm_ipc<VIDEO_SOURCE_BLOCK_NUM, VIDEO_SOURCE_BLOCK_SIZE> m_ipc;
    std::string m_id;
    bool m_ipcOwner;
    const static uint32_t s_ipcChannelNum;
    const static uint32_t s_ipcSourceWriteChannelId;
    const static uint32_t s_ipcSourceReadChannelId;
};

const uint32_t AgoraVideoSourceIpcImpl::s_ipcChannelNum = 2;
const uint32_t AgoraVideoSourceIpcImpl::s_ipcSourceWriteChannelId = 0;
const uint32_t AgoraVideoSourceIpcImpl::s_ipcSourceReadChannelId = 1;

AgoraVideoSourceIpcImpl::AgoraVideoSourceIpcImpl(AgoraIpcListener * listener)
    : IAgoraIpc(listener)
{
}

AgoraVideoSourceIpcImpl::~AgoraVideoSourceIpcImpl()
{
}

bool AgoraVideoSourceIpcImpl::initialize(const std::string & id)
{
    m_id = id;
    return true;
}

bool AgoraVideoSourceIpcImpl::listen()
{
    return false;
}

bool AgoraVideoSourceIpcImpl::connect()
{
    if (m_ipc.open(m_id) != 0) {
        return false;
    }

    if (m_ipc.open_channel(AgoraVideoSourceIpcImpl::s_ipcSourceWriteChannelId, CHANNEL_READ) != 0) {
        m_ipc.close();
        return false;
    }

    if (m_ipc.open_channel(AgoraVideoSourceIpcImpl::s_ipcSourceReadChannelId, CHANNEL_WRITE) != 0) {
        m_ipc.close_channel(AgoraVideoSourceIpcImpl::s_ipcSourceWriteChannelId, CHANNEL_READ);
        m_ipc.close();
        return false;
    }

    return true;
}

bool AgoraVideoSourceIpcImpl::disconnect()
{
    return false;
}

bool AgoraVideoSourceIpcImpl::run()
{
    char *readBuf = new char[VIDEO_SOURCE_BLOCK_SIZE];
    VideoSourceIpcMsgHeader *header;
    uint32_t fd = m_ipcOwner ? AgoraVideoSourceIpcImpl::s_ipcSourceReadChannelId : AgoraVideoSourceIpcImpl::s_ipcSourceWriteChannelId;
    while (true) {
        int ret = m_ipc.read(fd, readBuf, VIDEO_SOURCE_BLOCK_SIZE);
        if (ret < 0)
            break;
        if (m_listener) {
            header = (VideoSourceIpcMsgHeader *)readBuf;
            m_listener->onMessage(header->msg, readBuf + sizeof(VideoSourceIpcMsgHeader), header->len);
        }
    }
    delete[] readBuf;
    return true;
}

bool AgoraVideoSourceIpcImpl::sendMessage(AgoraIpcMsg msg, char * payload, unsigned int len)
{
    return false;
}


IAgoraIpc *createAgoraIpc(AgoraIpcListener *listener)
{
    return new AgoraVideoSourceIpcImpl(listener);
}

