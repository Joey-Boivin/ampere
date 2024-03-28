#include "amp_backend.h"
#include "amp_logger.h"

static struct amp_backend* backend;

#define DEBUG

#ifdef DEBUG
static enum log_level level = LOG_INFO;
#else
#include <stdio.h>
static enum log_level level = LOG_INFO;
#define LOG_FILE "logs.txt"
#endif


int
main(void)
{
#ifdef DEBUG
        amp_logger_init(level, stdout);
#else

        FILE* fp = fopen(LOG_FILE, "w");
        if (fp)
        {
                amp_logger_init(level, fp);
        }

#endif
        backend = amp_backend_connect();
        amp_backend_start(backend); /* blocking */
}
