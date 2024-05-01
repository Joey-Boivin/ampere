#ifndef AMP_BACKEND_ACTION
#define AMP_BACKEND_ACTION

enum amp_backend_action_type
{
    ACTION_WINDOW_RESIZE,
    ACTION_WINDOW_MOVE
};

struct amp_backend_action_resize {
    unsigned width;
    unsigned height;
};

struct amp_backend_action_move {
    unsigned x;
    unsigned y;
};

struct amp_backend_action
{
    enum amp_backend_action_type action_type;
    unsigned long window_id;
    union
    {
        struct amp_backend_action_resize resize_action;
        struct amp_backend_action_move move_action;
    } action;
};

#endif //AMP_BACKEND_ACTION
