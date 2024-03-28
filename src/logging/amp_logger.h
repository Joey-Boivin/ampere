#ifndef AMP_LOGGER_H
#define AMP_LOGGER_H

#include <stdio.h>

enum log_level
{
        LOG_INFO,
        LOG_WARN,
        LOG_ERROR,
};

#define AMP_LOGGER_INFO(...)  amp_logger_log(LOG_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define AMP_LOGGER_WARN(...)  amp_logger_log(LOG_WARN, __FILE__, __LINE__, __VA_ARGS__)
#define AMP_LOGGER_ERROR(...) amp_logger_log(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)

void amp_logger_init(enum log_level minimum_level, FILE* file_handle);
void amp_logger_log(enum log_level, const char* file, int line, const char* format, ...);

#endif // AMP_LOGGER_H
