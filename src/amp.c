#include "amp_backend.h"
#include "amp_logger.h"

#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

static struct amp_backend* backend;

#define DEBUG

#ifdef DEBUG
static enum log_level level = LOG_INFO;
#else
#include <stdio.h>
static enum log_level level = LOG_INFO;
#define LOG_FILE "logs.txt"
#endif

static void _init_logger(void)
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
}

static void _cleanup(void)
{
        amp_backend_disconnect(backend);

#ifndef DEBUG
        /* Because atexit could use stdout, prevent fclose in DEBUG mode */
        fclose(fp);
#endif

}

static bool _should_exit(int signal)
{
        return signal == SIGINT || signal == SIGQUIT || signal == SIGTERM;
}

static void _signal_handler(int signal)
{
        AMP_LOGGER_INFO("Received signal %d\n", signal);
        if (_should_exit(signal))
        {
                _cleanup();
                exit(EXIT_SUCCESS);
        }
}

static void _init_keyboard_interrupt_signal(const struct sigaction* sa)
{
    if (sigaction(SIGINT, sa, NULL) < 0)
    {
        AMP_LOGGER_WARN("Could not initialize keyboard interrupt signal\n");
    }
}

static void _init_quit_signal(const struct sigaction* sa)
{
        if (sigaction(SIGQUIT, sa, NULL) < 0)
        {
                AMP_LOGGER_WARN("Could not initialize quit signal\n");
        }
}

static void _init_termination_signal(const struct sigaction* sa)
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

        _init_logger();

        memset(&sa, 0x0, sizeof(sa));
        sa.sa_handler = _signal_handler;
        _init_keyboard_interrupt_signal(&sa);
        _init_quit_signal(&sa);
        _init_termination_signal(&sa);
        while(1);
        atexit(_cleanup);

        backend = amp_backend_connect();
        amp_backend_start(backend); /* blocking */
}
