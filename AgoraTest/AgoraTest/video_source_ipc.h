#ifndef VIDEO_SOURCE_IPC_H
#define VIDEO_SOURCE_IPC_H

#include <string>

/**
* AgoraIpcMsg define the message type transferred between node ADDON and vidoe source process
*/
enum AgoraIpcMsg
{
    /**  obsolete*/
    AGORA_IPC_CONNECT = 1,
    /** obsolete  */
    AGORA_IPC_CONNECT_CONFIRM,
    /** obsolete  */
    AGORA_IPC_DISCONNECT,
    /** To notify video source to join channel */
    AGORA_IPC_JOIN,
    /** To notify Video Sink that Video Source is ready*/
    AGORA_IPC_SOURCE_READY,
    /** Video source ==> node ADDON join success event */
    AGORA_IPC_JOIN_SUCCESS,
    /** Node ADDON ==> video source to begin capture screen */
    AGORA_IPC_CAPTURE_SCREEN,
    /** Node ADDON ==> video source to update screen sharing area*/
    AGORA_IPC_UPDATE_CAPTURE_SCREEN,
    /** Node ADDON ==> video source to stop capture screen */
    AGORA_IPC_STOP_CAPTURE_SCREEN,
    /** Node ADDON ==> video source to start video */
    AGORA_IPC_START_CAMERA,
    /** Node ADDON ==> video source to update token */
    AOGRA_IPC_RENEW_TOKEN,
    /** Node ADDON ==> video source to set channel profile */
    AGORA_IPC_SET_CHANNEL_PROFILE,
    /** Node ADDON ==> video source to set video profile */
    AGORA_IPC_SET_VIDEO_RPOFILE,
    /** Node ADDON ==> video source to leave channel */
    AGORA_IPC_LEAVE_CHANNEL,
    /** video source render is ready */
    AGORA_IPC_RENDER_READY,
    /**  Node ADDON ==> video source, To start transfer local video of video source.*/
    AGORA_IPC_START_VS_PREVIEW,
    /** video source ==> Node ADDON, local video preview complete.*/
    AGORA_IPC_START_VS_PREVIEW_COMPLETE,
    /** Node ADDON ==> video source, to stop transfer local video of video source. */
    AGORA_IPC_STOP_VS_PREVIEW,
    /** video source ==> Node ADDON, local video preview stopped.*/
    AGORA_IPC_STOP_VS_PREVIEW_COMPLETE
};

class AgoraIpcListener
{
public:
    virtual ~AgoraIpcListener() {}
    virtual void onMessage(unsigned int msg, char *payload, unsigned int len) {
        (void)payload;
        (void)len;
    }
};

class IAgoraIpc
{
public:
    IAgoraIpc(AgoraIpcListener *listener)
        : m_listener(listener)
    {}
    virtual ~IAgoraIpc() {}

    virtual bool initialize(const std::string &id) = 0;
    virtual bool listen() = 0;
    virtual bool connect() = 0;
    virtual bool disconnect() = 0;
    virtual bool run() = 0;
    virtual bool sendMessage(AgoraIpcMsg msg, char *payload, unsigned int len) = 0;

private:
    AgoraIpcListener *m_listener;
};

IAgoraIpc *createAgoraIpc(AgoraIpcListener *listener);

#endif
