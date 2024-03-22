#include "amp_backend.h"
#include "amp_backend_message.h"
#include "amp_wm.h"

#include <X11/Xlib.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define ROOT_WINDOW_INPUT_MASK (SubstructureRedirectMask | SubstructureNotifyMask | KeyPressMask |\
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

static void _amp_backend_x11_handle_key_press(XEvent* evt);
static void _amp_backend_x11_handle_configure_request(XEvent* evt);
static void _amp_backend_x11_handle_unmap_notify(XEvent* evt);
static void _amp_backend_x11_handle_map_request(XEvent* evt);

/* WARNING: The following syntax only works with gcc */
static void (*_amp_backend_x11_handle_event[LASTEvent])(XEvent*) = {
        [KeyPress]         = _amp_backend_x11_handle_key_press,
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

        struct amp_backend_message_ready ready = {
                .display_width = XDisplayWidth(backend->display, backend->default_screen),
                .display_height = XDisplayHeight(backend->display, backend->default_screen),
        };
        struct amp_backend_message message = {
                .message_type = MESSAGE_BACKEND_READY,
                .window_id = backend->root_window,
                .message.ready_message = ready
        };
        amp_wm_push_message(&message);

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

        while (XNextEvent(backend->display, &event) == 0)
        {
            if (_amp_backend_x11_handle_event[event.type])
            {
                _amp_backend_x11_handle_event[event.type](&event);
            }
        }

        return 0;
}

void
amp_backend_disconnect(struct amp_backend* backend)
{
        XCloseDisplay(backend->display);
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
        XSelectInput(backend->display, backend->root_window, ROOT_WINDOW_INPUT_MASK);
        /* Flush output buffer and wait for request to be processed */
        XSync(backend->display, 0);
}

static void
_amp_backend_x11_handle_key_press(XEvent* evt)
{
        printf("[AMPERE X11 BACKEND]: Key pressed\n");
        struct amp_backend_message_key_pressed key_pressed = {
                .key_code = evt->xkey.keycode,
                .modifier_key = evt->xkey.state
        };

        struct amp_backend_message message = {
                .message_type = MESSAGE_KEY_PRESSED,
                .window_id = evt->xkey.window,
                .message.key_pressed_message = key_pressed
        };

        amp_wm_push_message(&message);
}

static void
_amp_backend_x11_handle_configure_request(XEvent* evt)
{
    printf("[AMPERE X11 BACKEND]: Configure request\n");
    XWindowChanges changes;
    changes.x            = evt->xconfigurerequest.x;
    changes.y            = evt->xconfigurerequest.y;
    changes.width        = evt->xconfigurerequest.width;
    changes.height       = evt->xconfigurerequest.height;
    changes.border_width = evt->xconfigurerequest.border_width;
    changes.sibling      = evt->xconfigurerequest.above;
    changes.stack_mode   = evt->xconfigurerequest.detail;

    XConfigureWindow(evt->xconfigurerequest.display, evt->xconfigurerequest.window, evt->xconfigurerequest.value_mask, &changes);
}

static void
_amp_backend_x11_handle_unmap_notify(XEvent* evt)
{
        printf("[AMPERE X11 BACKEND]: Unmap notify\n");
        struct amp_backend_message_remove_window remove_window = {
                .unused = 0
        };

        struct amp_backend_message message = {
                .message_type = MESSAGE_WINDOW_REMOVE,
                .window_id = evt->xunmap.window,
                .message.window_remove_message = remove_window
        };

    amp_wm_push_message(&message);
}

static void
_amp_backend_x11_handle_map_request(XEvent* evt)
{
    printf("[AMPERE X11 BACKEND]: Map request\n");
    struct amp_backend_message_create_window create_window = {
            .unused = 0
    };

    struct amp_backend_message message = {
            .message_type = MESSAGE_WINDOW_CREATE,
            .window_id = evt->xmap.window,
            .message.window_create_message = create_window
    };

    amp_wm_push_message(&message);
}
