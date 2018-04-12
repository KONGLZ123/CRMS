#ifndef VIDEO_SOURCE_RENDER_H
#define VIDEO_SOURCE_RENDER_H

//#include "IAgoraRtcEngine.h"
#include "IAgoraMediaEngine.h"
//#include "video_source.h"

class AgoraVideoSource;

using agora::media::IExternalVideoRenderFactory;
using agora::media::IExternalVideoRender;
using agora::media::ExternalVideoRenerContext;
using agora::media::IVideoFrame;

class AgoraVideoSourceRenderFactory : public IExternalVideoRenderFactory
{
public:
    AgoraVideoSourceRenderFactory(AgoraVideoSource &videoSource);
    ~AgoraVideoSourceRenderFactory();

    virtual IExternalVideoRender* createRenderInstance(const ExternalVideoRenerContext& context) override;

private:
    AgoraVideoSource &m_videoSource;
};

#endif
