#ifndef AMP_WM_H
#define AMP_WM_H

#include "amp_backend_event.h"

struct amp_wm;

struct amp_wm* amp_wm_init(void);

void amp_wm_push_event(struct amp_wm* wm, const struct amp_backend_event* event);

#endif // AMP_WM_H
