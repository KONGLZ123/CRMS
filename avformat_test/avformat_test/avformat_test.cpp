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

#if 1
struct buffer_data {
    uint8_t *ptr;
    size_t size;
};

// 此函数回调只有在avformat_open_input时才回调
int read_packet(void *opaque, uint8_t *buf, int buf_size)
{
    buffer_data *bd = static_cast<buffer_data *>(opaque);
    buf_size = FFMIN(buf_size, bd->size);   // 注意，此处要特别处理不满4096的末尾数据

    if (!buf_size)
        return AVERROR_EOF;

    // 将文件缓存拷贝到IO上下文buf中
    memcpy(buf, bd->ptr, buf_size);
    bd->ptr  += buf_size;
    bd->size -= buf_size;

    printf("read_packet: %p, %d\n", bd->ptr, bd->size);

    return buf_size;    // 注意返回值要返回当前复制数据的大小
}

int main(int argc, char *argv[])
{
    av_register_all();

    char *filename = "cuc_ieschool.mkv";
    uint8_t *buffer;
    size_t size;
    // 将文件映射到buffer缓存
    if (av_file_map(filename, &buffer, &size, 0, NULL) < 0) {
        printf("av_file_map error\n");
        return -1;
    }

    // 为将要传入的结构体赋值
    buffer_data bd = { 0 };
    bd.ptr = buffer;
    bd.size = size;

    size_t avioBufSize = 4096;
    uint8_t *avioBuf = static_cast<uint8_t *>(av_malloc(avioBufSize));
    // 分配IO上下文，设置函数回调
    AVIOContext *avioCtx = avio_alloc_context(avioBuf, avioBufSize, 0, &bd, &read_packet, NULL, NULL);
    if (!avioCtx) {
        printf("avio_alloc_context error\n");
        return -1;
    }

    AVFormatContext *fmtCtx = avformat_alloc_context();
    fmtCtx->pb = avioCtx;
    if (avformat_open_input(&fmtCtx, NULL, NULL, NULL) < 0) {
        printf("avformat_open_input error\n");
        return -1;
    }

    if (avformat_find_stream_info(fmtCtx, NULL) < 0) {
        printf("avformat_find_stream_info error\n");
        return -1;
    }

    // 输出文件信息，fmtCtx是输入参数，故第四个参数为0
    av_dump_format(fmtCtx, 0, filename, 0);

    avformat_close_input(&fmtCtx);
    if (!avioCtx) {
        av_freep(avioCtx->buffer);
        av_freep(avioCtx);
    }
    av_file_unmap(buffer, size);

    return 0;
}

#endif







#if 0
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

    char *inputFilename = "cuc_ieschool.mkv";
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

#endif
