#include "amp_logger.h"

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>


#define ANSI_WHITE "\x1B[0;37m"
#define ANSI_YELLOW "\x1B[0;33m"
#define ANSI_RED "\x1B[0;31m"
#define COLOR_RESET "\x1B[0m"

#define MAX_HEADER_LENGTH 256

struct amp_logger {
    enum log_level minimum_level;
    FILE* fp;
};

static struct amp_logger logger = {
        .minimum_level = LOG_INFO,
        .fp = NULL,
};

static char* level_text[] = {
        "[INFO]   ",
        "[WARNING]",
        "[ERROR]  ",
};

static char* ansi_escape_codes [] = {
        ANSI_WHITE,
        ANSI_YELLOW,
        ANSI_RED,
};

void amp_logger_init(enum log_level minimum_level, FILE* fp)
{
    logger.minimum_level = minimum_level;
    logger.fp = fp;
}

void amp_logger_log(enum log_level level, const char* file, int line, const char* format, ...)
{
    char header[MAX_HEADER_LENGTH] = {0};
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    va_list args;

    if (level < logger.minimum_level || !logger.fp)
    {
        return;
    }

    if (logger.fp == stdout)
    {
        printf("%s", ansi_escape_codes[level]);
    }

    snprintf(header, MAX_HEADER_LENGTH - 1, "[%d-%02d-%02d %02d:%02d:%02d] %s %s:%d\t", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, level_text[level], file, line);
    va_start(args, format);
    fprintf(logger.fp, "%s", header);
    vfprintf(logger.fp, format, args);
    fprintf(logger.fp, "\n");
    va_end(args);


    if (logger.fp == stdout)
    {
        printf("%s", COLOR_RESET);
    }
}
