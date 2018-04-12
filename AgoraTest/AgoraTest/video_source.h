#ifndef VIDEO_SOURCE_H
#define VIDEO_SOURCE_H

#include <string>
#include <memory>
#include "IAgoraRtcEngine.h"
#include "video_source_ipc.h"
#include "video_source_param_parser.h"
#include "video_source_ipc.h"
#include "video_source_event_handler.h"
#include "video_source_render.h"

class AgoraVideoSource : public AgoraIpcListener
{
public:
    AgoraVideoSource(const std::string& params);
    ~AgoraVideoSource();

    bool initialize();
    void run();
    void release();
    void exit(bool notifySink);

private:
    bool m_initialized;
    std::string m_params;
    std::unique_ptr<AgoraVideoSourceEventHandler> m_eventHandler;
    std::unique_ptr<AgoraVideoSourceRenderFactory> m_renderFactory;
    std::unique_ptr<VideoSourceParamParser> m_paramParser;
    std::unique_ptr<IAgoraIpc> m_ipc;
    agora::util::AutoPtr<agora::rtc::IRtcEngine> m_rtcEngine;
    std::unique_ptr<INodeProcess> m_process;
    agora::rtc::VIDEO_PROFILE_TYPE m_videoProfile;
};

#endif