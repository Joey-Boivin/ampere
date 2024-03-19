#include "amp_backend.h"

#include <X11/Xlib.h>

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

struct amp_backend {
    Display* display;
    Window root_window;
    int default_screen;
    bool connected;
};

static int  _handle_x11_error(Display* display, XErrorEvent* err);
static void _try_connect(void);

struct amp_backend* amp_backend_connect(void)
{
    struct amp_backend* backend = malloc(sizeof(*backend));

    backend->display = XOpenDisplay(NULL);

    if (!backend->display)
    {
        printf("[AMPERE X11 BACKEND]: Could not open X display\n");
        free(backend);
        exit(EXIT_FAILURE);
    }

    backend->default_screen = XDefaultScreen(backend->display);
    backend->root_window = RootWindow(backend->display, backend->default_screen);
    XSetErrorHandler(_handle_x11_error);
    _try_connect();
    backend->connected = true;

    printf("[AMPERE X11 BACKEND]: Connection to X successful\n");
    return backend;
}

int amp_backend_start(struct amp_backend* backend)
{
    if (!backend->connected)
    {
        return -1;
    }
    XEvent event;

    while (XNextEvent(backend->display, &event) != 0)
    {
        break;
    }

    amp_backend_disconnect(backend);

    return 0;
}

void amp_backend_disconnect(struct amp_backend* backend)
{
    backend->connected = false;
    free(backend);
}

static int  _handle_x11_error(Display* display, XErrorEvent* err)
{
    switch (err->error_code)
    {
        case BadAccess:
        {
            printf("[AMPERE X11 BACKEND]: Another window manager is in control\n");
            XCloseDisplay(display);
            break;
        }
        default:
        {
            printf("[AMPERE X11 BACKEND]: Unhandled error (%d)\n", err->error_code);
        }
    }

    return 0;
}

static void _try_connect(void)
{

}