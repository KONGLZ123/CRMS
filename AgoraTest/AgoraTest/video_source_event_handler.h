#ifndef VIDEO_SOURCE_EVENT_HANDLER_H
#define VIDEO_SOURCE_EVENT_HANDLER_H

#include "IAgoraRtcEngine.h"
//#include "video_source.h"

class AgoraVideoSource;

using agora::rtc::IRtcEngineEventHandler;

class AgoraVideoSourceEventHandler : public IRtcEngineEventHandler
{
public:
    AgoraVideoSourceEventHandler(AgoraVideoSource &videoSource);
    ~AgoraVideoSourceEventHandler();

private:
    AgoraVideoSource &m_videoSource;
};

#endif