#include "amp_wm.h"
#include "amp_logger.h"
#include "amp_backend_event.h"

#include <stdlib.h>

struct amp_wm
{
    size_t monitor_width;
    size_t monitor_height;
};

static void _amp_wm_handle_backend_ready(struct amp_wm* wm, const struct amp_backend_event* event);
static void _amp_wm_handle_key_pressed(struct amp_wm* wm, const struct amp_backend_event* event);
static void _amp_wm_handle_window_create(struct amp_wm* wm, const struct amp_backend_event* event);
static void _amp_wm_handle_window_removed(struct amp_wm* wm, const struct amp_backend_event* event);
static void _amp_wm_handle_focus_in(struct amp_wm* wm, const struct amp_backend_event* event);

static void (*_amp_wm_handle_event[EVENT_TYPE_END])(struct amp_wm* wm, const struct amp_backend_event* event) = {
        [EVENT_BACKEND_READY]         = _amp_wm_handle_backend_ready,
        [EVENT_KEY_PRESSED] = _amp_wm_handle_key_pressed,
        [EVENT_WINDOW_CREATE]      = _amp_wm_handle_window_create,
        [EVENT_WINDOW_REMOVED]       = _amp_wm_handle_window_removed,
        [EVENT_FOCUS_IN] = _amp_wm_handle_focus_in
};

struct amp_wm*
amp_wm_init(void)
{
    struct amp_wm* wm = malloc(sizeof(*wm));
    if (!wm)
    {
        AMP_LOGGER_ERROR("Dynamic memory allocation error");
        exit(EXIT_FAILURE);
    }
    return wm;
}

void
amp_wm_push_event(struct amp_wm* wm, const struct amp_backend_event* event)
{
        _amp_wm_handle_event[event->event_type](wm, event);
}

static void _amp_wm_handle_backend_ready(struct amp_wm* wm, const struct amp_backend_event* event)
{
    wm->monitor_height = event->event.ready_event.display_height;
    wm->monitor_width = event->event.ready_event.display_width;
}

static void _amp_wm_handle_key_pressed(struct amp_wm* wm, const struct amp_backend_event* event)
{
    /* TODO */
}

static void _amp_wm_handle_window_create(struct amp_wm* wm, const struct amp_backend_event* event)
{
    /* TODO */
}

static void _amp_wm_handle_window_removed(struct amp_wm* wm, const struct amp_backend_event* event)
{
    /* TODO */
}

static void _amp_wm_handle_focus_in(struct amp_wm* wm, const struct amp_backend_event* event)
{
    /* TODO */
}
