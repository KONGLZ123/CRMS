// ffmpesg_decoder.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#define __STDC_CONSTANT_MACROS

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
}

int main()
{
    av_register_all();
    avformat_network_init();

    AVFormatContext *formatCtx = avformat_alloc_context();
    
    char filePath[] = "Titanic.ts";
    if (0 != avformat_open_input(&formatCtx, filePath, NULL, NULL)) {
        printf("Couldn't open input stream \n");
        return -1;
    }

    if (avformat_find_stream_info(formatCtx, NULL) < 0) {
        printf("Couldn't find stream info \n");
        return -1;
    }

    int videoIndex = -1;
    for (unsigned int i = 0; i < formatCtx->nb_streams; ++i) {
        if (formatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoIndex = i;
            break;
        }
    }
    if (-1 == videoIndex) {
        printf("Didn't find a video stream type\n");
        return -1;
    }

    AVCodecContext *codecCtx = formatCtx->streams[videoIndex]->codec;
    AVCodec *codec = avcodec_find_decoder(codecCtx->codec_id);
    if (NULL == codec) {
        printf("Codec not found\n");
        return -1;
    }

    if (avcodec_open2(codecCtx, codec, NULL) < 0) {
        printf("Could not open codec\n");
        return -1;
    }

    AVFrame *frame = av_frame_alloc();
    AVFrame *frameYuv = av_frame_alloc();
    uint8_t *outBuffer = (uint8_t *)av_malloc(avpicture_get_size(PIX_FMT_YUV420P, codecCtx->width, codecCtx->height));
    if (NULL == outBuffer) {
        printf("av_malloc failed \n");
        return -1;
    }

    avpicture_fill((AVPicture *)frameYuv, outBuffer, PIX_FMT_YUV420P, codecCtx->width, codecCtx->height);
    AVPacket *packet = (AVPacket *)av_malloc(sizeof(AVPacket));
    if (NULL == packet) {
        return -1;
    }

    printf("---------File Infomation---------\n");
    av_dump_format(formatCtx, 0, filePath, 0);
    printf("---------------------------------\n");

    struct SwsContext *img2ctx = sws_getContext(codecCtx->width, codecCtx->height, codecCtx->pix_fmt,
        codecCtx->width, codecCtx->height, PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);

    int frameCnt = 0;
    int gotPicture = 0;
    int ret = -1;
    while (av_read_frame(formatCtx, packet) >= 0) {
        if (packet->stream_index == videoIndex) {
            ret = avcodec_decode_video2(codecCtx, frame, &gotPicture, packet);
            if (ret < 0) {
                printf("Decode error \n");
                return -1;
            }
            if (gotPicture) {
                sws_scale(img2ctx, frame->data, frame->linesize, 0, codecCtx->height,
                    frameYuv->data, frameYuv->linesize);
                printf("Decoded frame index: %d\n", frameCnt);
                frameCnt++;
            }
        }
        av_free_packet(packet);
    }

    av_free_packet(packet);
    av_free(outBuffer);
    
    av_frame_free(&frameYuv);
    av_frame_free(&frame);
    avformat_close_input(&formatCtx);
    avformat_free_context(formatCtx);

    return 0;
}

