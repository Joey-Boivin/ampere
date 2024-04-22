#include "amp_backend.h"
#include "amp_logger.h"

#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static struct amp_backend* backend;

#define DEBUG

#ifdef DEBUG
static enum log_level level = LOG_INFO;
#else
#include <stdio.h>
static enum log_level level = LOG_INFO;
#define LOG_FILE "logs.txt"
#endif

static bool
_should_exit(int signal)
{
        return signal == SIGINT || signal == SIGQUIT || signal == SIGTERM;
}

static void
_signal_handler(int signal)
{
        if (_should_exit(signal))
        {
                amp_backend_stop(backend);
        }
}

static void
_init_keyboard_interrupt_signal(const struct sigaction* sa)
{
        if (sigaction(SIGINT, sa, NULL) < 0)
        {
                AMP_LOGGER_WARN("Could not initialize keyboard interrupt signal\n");
        }
}

static void
_init_quit_signal(const struct sigaction* sa)
{
        if (sigaction(SIGQUIT, sa, NULL) < 0)
        {
                AMP_LOGGER_WARN("Could not initialize quit signal\n");
        }
}

static void
_init_termination_signal(const struct sigaction* sa)
{
        if (sigaction(SIGTERM, sa, NULL) < 0)
        {
                AMP_LOGGER_WARN("Could not initialize termination signal\n");
        }
}

int
main(void)
{
        struct sigaction sa;

#ifdef DEBUG
        amp_logger_init(level, stdout);
#else

        FILE* fp = fopen(LOG_FILE, "w");
        if (fp)
        {
                amp_logger_init(level, fp);
        }
#endif

        memset(&sa, 0x0, sizeof(sa));
        sa.sa_handler = _signal_handler;
        _init_keyboard_interrupt_signal(&sa);
        _init_quit_signal(&sa);
        _init_termination_signal(&sa);

        backend = amp_backend_connect();
        amp_backend_start(backend); /* blocking until abnormal signal is received */
        amp_logger_close();

        return 0;
}
