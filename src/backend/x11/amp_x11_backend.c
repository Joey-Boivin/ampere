#include "amp_backend.h"
#include "amp_backend_event.h"
#include "amp_wm.h"
#include "amp_logger.h"

#include <X11/Xlib.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define ROOT_WINDOW_INPUT_MASK                                                                                         \
        (SubstructureRedirectMask | SubstructureNotifyMask | KeyPressMask | Mod4Mask | Mod3Mask | Mod2Mask |           \
         Mod1Mask | ControlMask | ShiftMask)

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
    [MapRequest]       = _amp_backend_x11_handle_map_request,
};

struct amp_backend*
amp_backend_connect(void)
{
        struct amp_backend* backend = malloc(sizeof(*backend));

        backend->display = XOpenDisplay(NULL);
        if (!backend->display)
        {
                AMP_LOGGER_ERROR("%s", "Could not open X display");
                free(backend);
                exit(EXIT_FAILURE);
        }
        backend->default_screen = XDefaultScreen(backend->display);
        backend->root_window    = RootWindow(backend->display, backend->default_screen);

        XSetErrorHandler(_amp_backend_x11_handle_error);
        _amp_backend_x11_try_connect(backend);
        backend->connected = true;

        struct amp_backend_event_ready ready = {
            .display_width  = XDisplayWidth(backend->display, backend->default_screen),
            .display_height = XDisplayHeight(backend->display, backend->default_screen),
        };

        struct amp_backend_event event = {
            .event_type        = EVENT_BACKEND_READY,
            .window_id         = backend->root_window,
            .event.ready_event = ready,
        };

        amp_wm_push_event(&event);

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

static int
_amp_backend_x11_handle_error(Display* display, XErrorEvent* err)
{
        (void)display;
        switch (err->error_code)
        {
        case BadAccess:
        {
                AMP_LOGGER_ERROR("%s", "Another window manager is in control");
                exit(EXIT_FAILURE);
        }
        default:
        {
                AMP_LOGGER_WARN("Unhandled error (%d)", err->error_code);
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
        AMP_LOGGER_INFO("%s", "Key pressed");
        struct amp_backend_event_key_pressed key_pressed = {
            .key_code     = evt->xkey.keycode,
            .modifier_key = evt->xkey.state,
        };

        struct amp_backend_event event = {
            .event_type              = EVENT_KEY_PRESSED,
            .window_id               = evt->xkey.window,
            .event.key_pressed_event = key_pressed,
        };

        amp_wm_push_event(&event);
}

static void
_amp_backend_x11_handle_configure_request(XEvent* evt)
{
        AMP_LOGGER_INFO("%s", "Configure request");
        XWindowChanges changes;
        changes.x            = evt->xconfigurerequest.x;
        changes.y            = evt->xconfigurerequest.y;
        changes.width        = evt->xconfigurerequest.width;
        changes.height       = evt->xconfigurerequest.height;
        changes.border_width = evt->xconfigurerequest.border_width;
        changes.sibling      = evt->xconfigurerequest.above;
        changes.stack_mode   = evt->xconfigurerequest.detail;

        XConfigureWindow(evt->xconfigurerequest.display, evt->xconfigurerequest.window,
                         evt->xconfigurerequest.value_mask, &changes);
}

static void
_amp_backend_x11_handle_unmap_notify(XEvent* evt)
{
        AMP_LOGGER_INFO("%s", "Unmap notify");
        struct amp_backend_event_remove_window remove_window = {
            .unused = 0,
        };

        struct amp_backend_event event = {
            .event_type                = EVENT_WINDOW_REMOVED,
            .window_id                 = evt->xunmap.window,
            .event.window_remove_event = remove_window,
        };

        amp_wm_push_event(&event);
}

static void
_amp_backend_x11_handle_map_request(XEvent* evt)
{
        AMP_LOGGER_INFO("%s", "Map request");
        struct amp_backend_event_create_window create_window = {
            .unused = 0,
        };

        struct amp_backend_event event = {
            .event_type                = EVENT_WINDOW_CREATE,
            .window_id                 = evt->xmap.window,
            .event.window_create_event = create_window,
        };

        amp_wm_push_event(&event);
}
