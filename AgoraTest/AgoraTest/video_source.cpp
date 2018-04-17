#include <string>
#include "node_process.h"
#include "node_log.h"
#include "video_source.h"

static const char* s_appid = "0c0b4b61adf94de1befd7cdd78a50444";

using agora::rtc::RtcEngineContext;

AgoraVideoSource::AgoraVideoSource(const std::string & params)
    : m_initialized(false)
    , m_params(params)
    , m_videoProfile(agora::rtc::VIDEO_PROFILE_DEFAULT)
{
    LOG_ENTER;
    LOG_LEAVE;
}

AgoraVideoSource::~AgoraVideoSource()
{
}

bool AgoraVideoSource::initialize()
{
    LOG_ENTER;

    m_paramParser.reset(new VideoSourceParamParser());
    m_paramParser->initialize(m_params);

    std::string id = m_paramParser->getParameter("id");
    if (id.empty()) {
        LOG_ERROR("%s, id is null\n", __FUNCTION__);
        LOG_LEAVE;
        return false;
    }

    m_ipc.reset(createAgoraIpc(this));
    if (!m_ipc->initialize(id)) {
        LOG_LEAVE;
        return false;
    }
    if (!m_ipc->connect()) {
        LOG_ERROR("%s, ipc connect fail.\n", __FUNCTION__);
        LOG_LEAVE;
        return false;
    }

    m_rtcEngine.reset(createAgoraRtcEngine());
    if (!m_rtcEngine.get()) {
        LOG_ERROR("%s, rtcengine create fail.\n", __FUNCTION__);
        LOG_LEAVE;
        return false;
    }

    m_eventHandler.reset(new AgoraVideoSourceEventHandler(*this));
    m_renderFactory.reset(new AgoraVideoSourceRenderFactory(*this));

    RtcEngineContext context;
    context.eventHandler = m_eventHandler.get();
    context.appId = s_appid;
    if (m_rtcEngine->initialize(context) != 0) {
        LOG_ERROR("%s, AgoraVideoSource initialized failed\n", __FUNCTION__);
        LOG_LEAVE;
        return false;
    }

    agora::util::AutoPtr<agora::media::IMediaEngine> pMediaEngine;
    pMediaEngine.queryInterface(m_rtcEngine.get(), agora::AGORA_IID_MEDIA_ENGINE);

    if (pMediaEngine.get()) {
        pMediaEngine->registerVideoRenderFactory(m_renderFactory.get());
    }
    else {
        LOG_ERROR("%s, Get media engine failed\n", __FUNCTION__);
        LOG_LEAVE;
        return false;
    }

    m_rtcEngine->disableAudio();
    m_rtcEngine->enableVideo();
    agora::rtc::RtcEngineParameters rep(m_rtcEngine.get());
    rep.enableLocalVideo(false);
    rep.muteAllRemoteVideoStreams(false);
    m_ipc->sendMessage(AGORA_IPC_SOURCE_READY, nullptr, 0);
    m_initialized = true;

    LOG_LEAVE;
    return false;
}

void AgoraVideoSource::run()
{
    LOG_ENTER;

    std::string idstr = m_paramParser->getParameter("pid");
    if (idstr.empty()) {
        LOG_ERROR("%s, pid is null\n", __FUNCTION__);
        LOG_LEAVE;
        return;
    }
    
    m_process.reset(INodeProcess::OpenNodeProcess(std::atoi(idstr.c_str())));
    if (!m_process.get()) {
        LOG_ERROR("%s, remote process exit\n", __FUNCTION__);
        LOG_LEAVE;
        return;
    }

    m_process->Monitor([this](INodeProcess *) {
        LOG_WARNNING("%s, remote process ext.\n", __FUNCTION__);
        this->exit(false);
    });
    m_ipc->run();

    LOG_LEAVE;
}

void AgoraVideoSource::release()
{
    delete this;
}

void AgoraVideoSource::exit(bool notifySink)
{
    //m_ipcSender.reset();
    m_ipc->disconnect();
}

void initLogService()
{
    std::string currentPath;
    INodeProcess::getCurrentModuleFileName(currentPath);
    std::string logFile = currentPath + ".txt";
    startLogService(logFile.c_str());
}

int main(int argc, char *argv[])
{
    initLogService();
    //if (argc < 3) {
    //    LOG_ERROR("Need at least 3 parameter. Current parameter num : %d\n", argc);
    //    return 0;
    //}

    std::string params;
    for (int i = 1; i < argc; ++i) {
        params.append(argv[i]);
        params.append(" ");
    }
    LOG_INFO("Params: %s\n", params.c_str());

    auto agoraVideoSource = new AgoraVideoSource(params);
    agoraVideoSource->initialize();
    agoraVideoSource->release();

    stopLogService();
}


