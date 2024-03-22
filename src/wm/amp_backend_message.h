#ifndef AMP_WM_STRUCTS_H
#define AMP_WM_STRUCTS_H

#include <stdbool.h>

/* TODO: Focus event, mouse pressed (or released) event, button pressed event != key pressed event, XPointerMovedEvent,  */
enum amp_backend_message_type
{
    MESSAGE_BACKEND_READY,
    MESSAGE_KEY_PRESSED,
    MESSAGE_WINDOW_CREATE,
    MESSAGE_WINDOW_REMOVE,
};

struct amp_backend_message_ready
{
    unsigned display_width;
    unsigned display_height;
};

struct amp_backend_message_key_pressed
{
    unsigned key_code;
    unsigned modifier_key;
};

struct amp_backend_message_create_window
{
    int unused;
};

struct amp_backend_message_remove_window
{
    int unused;
};

struct amp_backend_message
{
    enum amp_backend_message_type message_type;
    unsigned long window_id;
    union
    {
        struct amp_backend_message_ready ready_message;
        struct amp_backend_message_key_pressed key_pressed_message;
        struct amp_backend_message_create_window window_create_message;
        struct amp_backend_message_remove_window window_remove_message;
    } message;
};
#endif //AMP_WM_STRUCTS_H
