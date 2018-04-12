#include "video_source_render.h"

AgoraVideoSourceRenderFactory::AgoraVideoSourceRenderFactory(AgoraVideoSource & videoSource)
    : m_videoSource(videoSource)
{
}

AgoraVideoSourceRenderFactory::~AgoraVideoSourceRenderFactory()
{
}

IExternalVideoRender * AgoraVideoSourceRenderFactory::createRenderInstance(const ExternalVideoRenerContext & context)
{
    return nullptr;
}
