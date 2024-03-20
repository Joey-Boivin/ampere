#include "amp_backend.h"

#include <X11/Xlib.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define X11_INPUT_MASK (SubstructureRedirectMask | SubstructureNotifyMask | KeyPressMask |\
                        ButtonPressMask | Mod4Mask | Mod3Mask | Mod2Mask | Mod1Mask |\
                        ControlMask | ShiftMask)

struct amp_backend
{
        Display* display;
        Window   root_window;
        int      default_screen;
        bool     connected;
};

static int  _amp_backend_x11_handle_error(Display* display, XErrorEvent* err);
static void _amp_backend_x11_try_connect(const struct amp_backend* backend);

static void _amp_backend_x11_handle_button_press(XEvent* evt);
static void _amp_backend_x11_handle_key_press(XEvent* evt);
static void _amp_backend_x11_handle_configure_request(XEvent* evt);
static void _amp_backend_x11_handle_unmap_notify(XEvent* evt);
static void _amp_backend_x11_handle_map_request(XEvent* evt);

/* WARNING: The following syntax only works with gcc */
static void (*_amp_backend_x11_handle_event[LASTEvent])(XEvent*) = {
        [KeyPress]         = _amp_backend_x11_handle_key_press,
        [ButtonPress]      = _amp_backend_x11_handle_button_press,
        [ConfigureRequest] = _amp_backend_x11_handle_configure_request,
        [UnmapNotify]      = _amp_backend_x11_handle_unmap_notify,
        [MapRequest]       = _amp_backend_x11_handle_map_request};

struct amp_backend*
amp_backend_connect(void)
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
        backend->root_window    = RootWindow(backend->display, backend->default_screen);

        XSetErrorHandler(_amp_backend_x11_handle_error);
        _amp_backend_x11_try_connect(backend);
        backend->connected = true;
        printf("[AMPERE X11 BACKEND]: Connection to X successful\n");
        return backend;
}

int
amp_backend_start(struct amp_backend* backend)
{
        XEvent event;

        if (!backend->connected)
        {
                return -1;
        }

        while (XNextEvent(backend->display, &event) != 0)
        {
                if (_amp_backend_x11_handle_event[event.type])
                {
                        _amp_backend_x11_handle_event[event.type](&event);
                }
        }
        amp_backend_disconnect(backend);
        return 0;
}

void
amp_backend_disconnect(struct amp_backend* backend)
{
        backend->connected = false;
        free(backend);
}

static int
_amp_backend_x11_handle_error(Display* display, XErrorEvent* err)
{
        (void)display;
        switch (err->error_code)
        {
        case BadAccess:
        {
                printf("[AMPERE X11 BACKEND]: Another window manager is in control\n");
                exit(EXIT_FAILURE);
        }
        default:
        {
                printf("[AMPERE X11 BACKEND]: Unhandled error (%d)\n", err->error_code);
        }
        }

        return 0;
}

static void
_amp_backend_x11_try_connect(const struct amp_backend* backend)
{
        /* Queue request to redirect events of root window */
        XSelectInput(backend->display, backend->root_window, X11_INPUT_MASK);
        /* Flush output buffer and wait for request to be processed */
        XSync(backend->display, QueuedAlready);
}

static void
_amp_backend_x11_handle_button_press(XEvent* evt)
{
        (void)evt;
}
static void
_amp_backend_x11_handle_key_press(XEvent* evt)
{
        (void)evt;
}

static void
_amp_backend_x11_handle_configure_request(XEvent* evt)
{
        (void)evt;
}
static void
_amp_backend_x11_handle_unmap_notify(XEvent* evt)
{
        (void)evt;
}

static void
_amp_backend_x11_handle_map_request(XEvent* evt)
{
        (void)evt;
}
