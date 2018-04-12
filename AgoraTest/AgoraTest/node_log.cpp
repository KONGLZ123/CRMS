#include <stdio.h>
#include <stdarg.h>
#include "node_log.h"

#pragma warning(disable:4996)

static FILE *log_stream = nullptr;
static log_level s_log_level = LOG_LEVEL_INFO;

bool startLogService(const char *path)
{
    if (log_stream)
        return false;
    if (path) {
        log_stream = fopen(path, "w");
        return true;
    }
    return false;
}

void stopLogService()
{
    if (log_stream) {
        fflush(log_stream);
        fclose(log_stream);
        log_stream = nullptr;
    }
}

void setLogLevel(log_level level)
{
    s_log_level = level;
}


void node_log(log_level level, const char * format, ...)
{
    if (!log_stream)
        return;
    if (level > s_log_level)
        return;

    va_list va;
    va_start(va, format);
    vfprintf(log_stream, format, va);
    va_end(va);
    fprintf(log_stream, "\n");
    fflush(log_stream);
}
