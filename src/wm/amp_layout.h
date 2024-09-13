#ifndef AMP_LAYOUT_H
#define AMP_LAYOUT_H

#include "amp_backend_action.h"

#include <stddef.h>

enum window_movement_direction
{
        WINDOW_MOVE_UP,
        WINDOW_MOVE_RIGHT,
        WINDOW_MODE_DOWN,
        WINDOW_MODE_LEFT
};

struct wm_actions
{
        struct amp_backend_action* actions;
        size_t                     number_of_actions;
};

struct amp_wm_layout;

struct amp_wm_layout* amp_layout_init(unsigned monitor_width, unsigned monitor_height);

const struct wm_actions* amp_layout_add_window(struct amp_wm_layout* layout, unsigned long focused_window,
                                               unsigned long window_to_add_id);
const struct wm_actions* amp_layout_remove_window(struct amp_wm_layout* layout, unsigned long focused_window,
                                                  unsigned long window_to_remove_id);
const struct wm_actions* amp_layout_move_window(struct amp_wm_layout* layout, unsigned long focused_window,
                                                unsigned long window_to_move_id);

void amp_layout_destroy(struct amp_wm_layout* layout);

#endif // AMPERE_AMP_LAYOUT_H
