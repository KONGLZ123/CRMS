// filtering_video.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#define __STDC_CONSTANT_MACROS

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavutil/opt.h>
};

AVFormatContext *formatCtx = NULL;
AVCodecContext *decCtx = NULL;
static int videoStreamIndex = -1;

static int open_input_file(const char *filename)
{
    int ret;
    if ((ret = avformat_open_input(&formatCtx, filename, NULL, NULL)) < 0) {
        av_log(NULL, AV_LOG_ERROR, "Can't open input file\n");
        return ret;
    }

    if ((ret = avformat_find_stream_info(formatCtx, NULL)) < 0) {
        av_log(NULL, AV_LOG_ERROR, "Can't find stream information\n");
        return ret;
    }

    AVCodec *dec = NULL;
    ret = av_find_best_stream(formatCtx, AVMEDIA_TYPE_VIDEO, -1, -1, &dec, 0);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Can't find a video stream in the input file\n");
        return ret;
    }
    videoStreamIndex = ret;

    decCtx = avcodec_alloc_context3(dec);
    if (!decCtx) {
        return AVERROR(ENOMEM);
    }

    //avcodec_parameters_to_context(decCtx, formatCtx->streams[videoStreamIndex]->codecpar);
    av_opt_set_int(decCtx, "refcounted_frames", -1, 0);

    if ((ret = avcodec_open2(decCtx, dec, NULL)) < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot open video decoder\n");
        return ret;
    }

    return 0;
}

int main()
{
    AVFrame *frame = av_frame_alloc();
    AVFrame *filt_frame = av_frame_alloc();

    if (!frame || !filt_frame) {
        perror("Could not allocate frame");
        exit(1);
    }

    avfilter_register_all();

    const char *filename = "";
    int ret;
    if ((ret = open_input_file(filename)) < 0) {
        //fprintf(stderr, "Error occurred: %s\n", av_err2str(ret));
    }
    if ((ret = init_filters(filter_descr)) < 0) {

    }

    return 0;
}

