#include "video_source_ipc.h"
#include "ipc_shm.h"

#define VIDEO_SOURCE_BLOCK_NUM      10
#define VIDEO_SOURCE_BLOCK_SIZE     1024

class AgoraVideoSourceImpl : public IAgoraIpc
{
public:
    AgoraVideoSourceImpl(AgoraIpcListener *listener);
    ~AgoraVideoSourceImpl();

    virtual bool initialize(const std::string &id) override;
    virtual bool listen() override;
    virtual bool connect() override;
    virtual bool disconnect() override;
    virtual bool run() override;
    virtual bool sendMessage(AgoraIpcMsg msg, char *payload, unsigned int len) override;

private:
    shm_ipc<VIDEO_SOURCE_BLOCK_NUM, VIDEO_SOURCE_BLOCK_SIZE> m_ipc;
    std::string m_id;
};

AgoraVideoSourceImpl::AgoraVideoSourceImpl(AgoraIpcListener * listener)
    : IAgoraIpc(listener)
{
}

AgoraVideoSourceImpl::~AgoraVideoSourceImpl()
{
}

bool AgoraVideoSourceImpl::initialize(const std::string & id)
{
    m_id = id;
    return true;
}

bool AgoraVideoSourceImpl::listen()
{
    return false;
}

bool AgoraVideoSourceImpl::connect()
{
    if (m_ipc.open(m_id) != 0) {
        return false;
    }

    return false;
}

bool AgoraVideoSourceImpl::disconnect()
{
    return false;
}

bool AgoraVideoSourceImpl::run()
{
    return false;
}

bool AgoraVideoSourceImpl::sendMessage(AgoraIpcMsg msg, char * payload, unsigned int len)
{
    return false;
}


IAgoraIpc *createAgoraIpc(AgoraIpcListener *listener)
{
    return new AgoraVideoSourceImpl(listener);
}

