// video_play.cpp : �������̨Ӧ�ó������ڵ㡣
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
    
    AVFormatContext *formatCtx = avformat_alloc_context();
    const char *filename = "Titanic.ts";
    FILE * fileVideoInfo = NULL;

    // �����ļ�����¼��Ƶ��Ϣ
    fopen_s(&fileVideoInfo, "videoInfo.txt", "wb+");
    if (NULL == fileVideoInfo) {
        printf("Create file error\n");
        return -1;
    }
        
    // ����Ƶ������
    if (0 != avformat_open_input(&formatCtx, filename, NULL, NULL)) {
        printf("Open input error\n");
        return -1;
    }

    // ��ȡ��Ƶ����Ϣ
    if (avformat_find_stream_info(formatCtx, NULL) < 0) {
        printf("Find stream info error\n");
        return -1;
    }

    AVInputFormat *inputFormat = formatCtx->iformat;

    int index = 0;
    // �ж���һ·����Ƶ��
    for (unsigned int i = 0; i < formatCtx->nb_streams; ++i) {
        if (formatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            index = i;
            break;
        }
    }
   
    fprintf(fileVideoInfo, "�ļ���: %s\n", formatCtx->filename);
    fprintf(fileVideoInfo, "ʱ��: %lld\n", formatCtx->duration);
    fprintf(fileVideoInfo, "����: %d\n", formatCtx->bit_rate);
    fprintf(fileVideoInfo, "��չ��: %s\n", inputFormat->extensions);
    fprintf(fileVideoInfo, "��װ��ʽ: %s\n", inputFormat->name);
    fprintf(fileVideoInfo, "��װ��ʽ��������: %s\n", inputFormat->long_name);
    
    AVCodecContext *codecCtx = formatCtx->streams[index]->codec;
    // ���ҽ�����
    //AVCodec *codec = formatCtx->video_codec;
    AVCodec *codec = avcodec_find_decoder(codecCtx->codec_id);
    if (NULL == codec) {
        printf("find decoder error\n");
        return -1;
    }

    if (avcodec_open2(codecCtx, codec, NULL) < 0) {
        printf("avcodec open error\n");
        return -1;
    }

    AVFrame *frame = av_frame_alloc();
    AVFrame *frameYuv = av_frame_alloc();
    uint8_t *outBuffer = (uint8_t *)malloc(avpicture_get_size(PIX_FMT_YUV420P, codecCtx->width, codecCtx->height));
    avpicture_fill((AVPicture *)frameYuv, outBuffer, PIX_FMT_YUV420P, codecCtx->width, codecCtx->height);

    SwsContext *img_convert_ctx = sws_getContext(codecCtx->width, codecCtx->height, codecCtx->pix_fmt, 
        codecCtx->width, codecCtx->height, PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);
    AVPacket *packet = (AVPacket *)av_malloc(sizeof(AVPacket));

    // ������ʾ����
    SDL_Init(SDL_INIT_VIDEO);
    int screenWidth = codecCtx->width;
    int screenHeight = codecCtx->height;
    SDL_Window *sdlWnd = SDL_CreateWindow("DisplayVideoDemo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        screenWidth, screenHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    SDL_Renderer *render = SDL_CreateRenderer(sdlWnd, -1, 0);
    SDL_Texture *texture = SDL_CreateTexture(render, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING,
        screenWidth, screenHeight);

    SDL_Thread * thread = SDL_CreateThread(VideoShowThread, NULL, NULL);
    SDL_Event event;
    //SDL_Rect sdlRect;
    int got_picture;

    while (1) {
        SDL_WaitEvent(&event);
        if (event.type == DISPLAY_VIDEO) {
            if (av_read_frame(formatCtx, packet) == 0) {
                if (packet->stream_index == index) {
                    // packet => frame => frameYuv
                    avcodec_decode_video2(codecCtx, frame, &got_picture, packet);
                    if (got_picture) {
                        sws_scale(img_convert_ctx, frame->data, frame->linesize, 0,
                            codecCtx->height, frameYuv->data, frameYuv->linesize);

                        SDL_UpdateTexture(texture, NULL, frameYuv->data[0], frameYuv->linesize[0]);
                        SDL_RenderClear(render);
                        SDL_RenderCopy(render, texture, NULL, NULL);
                        SDL_RenderPresent(render);
                    }
                }
                av_free_packet(packet);
            }
            else {
                exitCode = 1;
            }
        }
        else if (event.type == SDL_QUIT) {
            exitCode = 1;
        }
        else if (event.type == BREAD_DISPALY) {
            break;
        }
    }

    sws_freeContext(img_convert_ctx);
    avformat_close_input(&formatCtx);
    avformat_free_context(formatCtx);
    avformat_network_deinit();
    SDL_Quit();

    return 0;
}

