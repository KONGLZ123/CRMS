// avformat_test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#define __STDC_CONSTANT_MACROS

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavformat/avio.h"
#include "libavutil/file.h"
}

struct buffer_data {
    uint8_t *ptr;
    size_t size;
};

static int ReadPacket(void *opaque, uint8_t *buf, int bufSize)
{
    struct buffer_data *bd = (struct buffer_data *)opaque;
    bufSize = FFMIN(bufSize, bd->size);

    if (!bufSize) {
        return AVERROR_EOF;
    }
    printf("ptr: %p, size: %zu\n", bd->ptr, bd->size);

    memcpy(buf, bd->ptr, bufSize);
    bd->ptr += bufSize;
    bd->size -= bufSize;

    return bufSize;
}


/**
* libavformat AVIOContext API example.
*
* Make libavformat demuxer access media content through a custom
* AVIOContext read callback.
*/
int main(int argc, char *argv[])
{
    av_register_all();

    char *inputFilename = "Forrest_Gump_IMAX.mp4";
    uint8_t *buffer = NULL;
    size_t bufferSize;
    int ret = av_file_map(inputFilename, &buffer, &bufferSize, 0, NULL);
    if (ret < 0) {
        //goto end;
    }

    struct buffer_data bd = { 0 };
    bd.ptr = buffer;
    bd.size = bufferSize;
    AVFormatContext *fmtCtx = avformat_alloc_context();
    if (!fmtCtx) {
        //goto end;
    }

    size_t avioCtxBufferSize = 4096;
    uint8_t *avioCtxBuffer = (uint8_t *)av_malloc(avioCtxBufferSize);
    if (!avioCtxBuffer) {
        //goto end;
    }

    AVIOContext *avioCtx = avio_alloc_context(avioCtxBuffer, avioCtxBufferSize, 0, 
        &bd, &ReadPacket, NULL, NULL);
    if (!avioCtx) {
        //goto end;
    }

    fmtCtx->pb = avioCtx;
    if (avformat_open_input(&fmtCtx, NULL, NULL, NULL) < 0) {
        //goto end;
    }

    if (avformat_find_stream_info(fmtCtx, NULL) < 0) {
        //goto end;
    }

    av_dump_format(fmtCtx, 0, inputFilename, 0);

//end:
    avformat_close_input(&fmtCtx);
    if (avioCtx) {
        av_freep(&avioCtx->buffer);
        av_freep(&avioCtx);
    }
    av_file_unmap(buffer, bufferSize);

    return 0;
}


