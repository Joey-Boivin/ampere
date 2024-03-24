#ifndef AMP_BACKEND_EVENT
#define AMP_BACKEND_EVENT

#include <stdbool.h>

/* TODO: Focus event, mouse pressed (or released) event, button pressed event != key pressed event, XPointerMovedEvent,
 */
enum amp_backend_event_type
{
        EVENT_BACKEND_READY,
        EVENT_KEY_PRESSED,
        EVENT_WINDOW_CREATE,
        EVENT_WINDOW_REMOVED,
        EVENT_FOCUS_IN
};

struct amp_backend_event_ready
{
        unsigned display_width;
        unsigned display_height;
};

struct amp_backend_event_key_pressed
{
        unsigned key_code;
        unsigned modifier_key;
};

struct amp_backend_event_create_window
{
        int unused;
};

struct amp_backend_event_remove_window
{
        int unused;
};

struct amp_backend_event_focus_in
{
    int unused;
};

struct amp_backend_event
{
        enum amp_backend_event_type event_type;
        unsigned long               window_id;
        union
        {
                struct amp_backend_event_ready         ready_event;
                struct amp_backend_event_key_pressed   key_pressed_event;
                struct amp_backend_event_create_window window_create_event;
                struct amp_backend_event_remove_window window_remove_event;
                struct amp_backend_event_focus_in focus_in_event;
        } event;
};
#endif // AMP_BACKEND_EVENT
