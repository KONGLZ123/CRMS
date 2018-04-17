#ifndef NODE_LOG_H
#define NODE_LOG_H

enum log_level
{
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARNNING,
    LOG_LEVEL_INFO,
    LOG_LEVEL_VERBOSE
};

bool startLogService(const char *path);

void stopLogService();

void setLogLevel(log_level level);

void node_log(enum log_level level, const char *format, ...);

#define LOG_ERROR(format, ...)          node_log(LOG_LEVEL_ERROR, format, ##__VA_ARGS__)

#define LOG_WARNNING(format, ...)       node_log(LOG_LEVEL_WARNNING, format, ##__VA_ARGS__)

#define LOG_INFO(format, ...)           node_log(LOG_LEVEL_INFO, format, ##__VA_ARGS__)

#define LOG_VERBOSE(format, ...)        node_log(LOG_LEVEL_VERBOSE, format, ##__VA_ARGS__)

#define LOG_ENTER       LOG_VERBOSE("==> %s", __FUNCTION__)

#define LOG_LEAVE       LOG_VERBOSE("<== %s", __FUNCTION__)

#endif
