#ifndef __SLOG_H
#define __SLOG_H

#include "main.h"

#define SLOG_ASYNC_SEND 0

#define SLOG_BUFFER_SIZE 128

#if SLOG_ASYNC_SEND == 1
#define SLOG_RING_BUFFER_SIZE 1024
#define SLOG_MAX_WAIT_CNT 0xffffff
#endif

typedef enum
{
    SLOG_NORMAL = 0,
    SLOG_INFO,
    SLOG_DEBUG,
    SLOG_WARNING,
    SLOG_ERROR,
} sLogLevelDef;

typedef enum
{
    SLOG_SUCCESS = 0,
    SLOG_FULL,
} sLogState;

#if SLOG_ASYNC_SEND == 1
void sLog_async_send_cplt_callback(void);
#endif

void sLog_init(void);
sLogState sLog_print(const char *__format, ...);
sLogState sLog_print_with_time(const char *__format, ...);
sLogState sLog_print_with_level(sLogLevelDef level, const char *__format, ...);
sLogState sLog_print_with_time_level(sLogLevelDef level, const char *__format, ...);

#endif
