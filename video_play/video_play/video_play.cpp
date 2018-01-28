// video_play.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#define __STDC_CONSTANT_MICROS

extern "C"
{
#include "SDL2/sdl.h"
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
#include "SDL2/sdl.h"
}

#define DISPLAY_VIDEO       (SDL_USEREVENT + 1)
#define BREAD_DISPALY       (SDL_USEREVENT + 2)

int exitCode = 0;
int VideoShowThread(void *data)
{
    SDL_Event event;
    while (!exitCode) {
        event.type = DISPLAY_VIDEO;
        SDL_PushEvent(&event);
        SDL_Delay(40);
    }

    event.type = BREAD_DISPALY;
    SDL_PushEvent(&event);

    return 0;
}

int main()
{
    av_register_all();
    avformat_network_init();
    SDL_Init(SDL_INIT_VIDEO);

    AVFormatContext *formatCtx = avformat_alloc_context();
    const char *filename = "Titanic.ts";
    FILE * fileVideoInfo = NULL;

    // 创建文件，记录视频信息
    fopen_s(&fileVideoInfo, "videoInfo.txt", "wb+");
    if (NULL == fileVideoInfo) {
        printf("Create file error\n");
        return -1;
    }
        
    // 打开视频输入流
    if (0 != avformat_open_input(&formatCtx, filename, NULL, NULL)) {
        printf("Open input error\n");
        return -1;
    }

    // 获取视频流信息
    if (avformat_find_stream_info(formatCtx, NULL)) {
        printf("Find stream info error\n");
        return -1;
    }

    AVInputFormat *inputFormat = formatCtx->iformat;
    // 查找解码器
    //AVCodec *codec = formatCtx->video_codec;
    AVCodec *codec = avcodec_find_decoder(formatCtx->video_codec_id);
    if (NULL == codec) {
        printf("find decoder error\n");
        return -1;
    }

    int index = 0;
    // 判断那一路是视频流
    for (unsigned int i = 0; i < formatCtx->nb_streams; ++i) {
        if (codec->type == AVMEDIA_TYPE_VIDEO) {
            index = i;
            break;
        }
    }
   
    fprintf(fileVideoInfo, "文件名: %s\n", formatCtx->filename);
    fprintf(fileVideoInfo, "时长: %lld\n", formatCtx->duration);
    fprintf(fileVideoInfo, "码率: %d\n", formatCtx->bit_rate);
    fprintf(fileVideoInfo, "扩展名: %s\n", inputFormat->extensions);
    fprintf(fileVideoInfo, "封装格式: %s\n", inputFormat->name);
    fprintf(fileVideoInfo, "封装格式（完整）: %s\n", inputFormat->long_name);
    
    AVCodecContext *codecCtx = NULL;
    if (avcodec_open2(codecCtx, codec, NULL)) {
        printf("avcodec open error\n");
        return -1;
    }

    // 创建显示窗口
    int screenWidth = codecCtx->width;
    int screenHeight = codecCtx->height;
    SDL_Window *sdlWnd = SDL_CreateWindow("DisplayVideoDemo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        screenWidth, screenHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    SDL_Renderer *render = SDL_CreateRenderer(sdlWnd, -1, SDL_RENDERER_TARGETTEXTURE);
    SDL_Texture *texture = SDL_CreateTexture(render, PIX_FMT_YUV420P, SDL_TEXTUREACCESS_STREAMING,
        screenWidth, screenHeight);

    SDL_Thread * thread = SDL_CreateThread(VideoShowThread, NULL, NULL);
    SDL_Event event;
    AVFrame *frame = av_frame_alloc();
    AVPacket *packet = NULL;
    int got_picture;
    SDL_Rect sdlRect;

    
    while (!av_read_frame(formatCtx, packet)) {
        SDL_WaitEvent(&event);
        if (event.type == DISPLAY_VIDEO) {
            avcodec_decode_video2(codecCtx, frame, &got_picture, packet);
            if (got_picture) {
                SwsContext *swsCtx = sws_getContext(codecCtx->width, codecCtx->height,
                    PIX_FMT_VDPAU_H264, frame->width, frame->height, PIX_FMT_YUV420P,
                    0, NULL, NULL, NULL);
                //sws_scale(swsCtx, packet->data, , frame->height, frame->data, frame->linesize);
            }

            sdlRect.x = 0;
            sdlRect.y = 0;
            sdlRect.w = frame->width;
            sdlRect.h = frame->height;
            SDL_UpdateTexture(texture, NULL, frame->data[0], frame->linesize);

            SDL_RenderClear(render);
            SDL_RenderCopy(render, texture, NULL, &sdlRect);
            SDL_RenderPresent(render);
        }
        else if (event.type == BREAD_DISPALY) {
            exitCode = 1;
            break;
        }
    }

    avformat_close_input(&formatCtx);
    avformat_free_context(formatCtx);
    avformat_network_deinit();
    SDL_Quit();

    return 0;
}

