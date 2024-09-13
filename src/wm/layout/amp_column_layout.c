#include "amp_layout.h"
#include "amp_logger.h"

#include <stdlib.h>

struct window
{
        unsigned long window_id;
        unsigned      width;
        unsigned      height;
        unsigned      x_position;
        unsigned      y_position;
};

struct window_node
{
        struct window       win;
        struct window_node* previous;
        struct window_node* next;
};

struct column_root
{
        struct window_node* node;
        struct column_root* left;
        struct column_root* right;
};

struct amp_wm_layout
{
        struct column_root* first_column;
        unsigned            monitor_width;
        unsigned            monitor_height;
};

static struct window_node* _find_window(const struct amp_wm_layout* layout, unsigned long id_to_find);

static void _add_first_window(struct amp_wm_layout* layout, unsigned long window_to_add_id);
static void _add_second_window(struct amp_wm_layout* layout, unsigned long window_to_add_id);
static void _add_window(struct amp_wm_layout* layout, unsigned long focused_window, unsigned long window_to_add_id);

struct amp_wm_layout*
amp_layout_init(unsigned monitor_width, unsigned monitor_height)
{
        struct amp_wm_layout* layout = malloc(sizeof(*layout));
        if (!layout)
        {
                AMP_LOGGER_ERROR("Dynamic memory allocation error");
                exit(EXIT_FAILURE);
        }

        layout->first_column   = NULL;
        layout->monitor_width  = monitor_width;
        layout->monitor_height = monitor_height;

        return layout;
}

const struct wm_actions*
amp_layout_add_window(struct amp_wm_layout* layout, unsigned long focused_window_id, unsigned long window_to_add_id)
{
        struct window_node* node = _find_window(layout, focused_window_id);

        if (!node)
        {

                _add_first_window(layout, window_to_add_id);
                /* TODO: return actions */
                return NULL;
        }
        else if ((struct window_node*)layout->first_column == node)
        {
                _add_second_window(layout, window_to_add_id);
                /* TODO: return actions */
                return NULL;
        }
        else
        {
                _add_window(layout, focused_window_id, window_to_add_id);
                /* TODO: return actions */
                return NULL;
        }
}

static struct window_node*
_find_window(const struct amp_wm_layout* layout, unsigned long id_to_find)
{
        struct column_root* root = layout->first_column;

        while (root)
        {
                if (root->node->win.window_id == id_to_find)
                {
                        return (struct window_node*)root;
                }

                struct window_node* node = root->node->next;
                while (node)
                {
                        if (node->win.window_id == id_to_find)
                        {
                                return node;
                        }
                        node = node->next;
                }
                root = root->right;
        }
        return NULL;
}

static void
_add_first_window(struct amp_wm_layout* layout, unsigned long window_to_add_id)
{
        struct column_root* new_root = malloc(sizeof(*new_root));
        if (!new_root)
        {
                AMP_LOGGER_ERROR("Dynamic memory allocation error");
                exit(EXIT_FAILURE);
        }

        new_root->left       = NULL;
        new_root->right      = NULL;
        new_root->node->next = malloc(sizeof(*new_root->node->next));
        if (!new_root->node->next)
        {
                AMP_LOGGER_ERROR("Dynamic memory allocation error");
                exit(EXIT_FAILURE);
        }

        new_root->node->next->win.window_id = window_to_add_id;

        new_root->node->win.width      = layout->monitor_width;
        new_root->node->win.height     = layout->monitor_height;
        new_root->node->win.x_position = 0;
        new_root->node->win.y_position = 0;

        layout->first_column = new_root;
}

static void
_add_second_window(struct amp_wm_layout* layout, unsigned long window_to_add_id)
{
        struct column_root* new_root = malloc(sizeof(*new_root));
        if (!new_root)
        {
                AMP_LOGGER_ERROR("Dynamic memory allocation error");
                exit(EXIT_FAILURE);
        }

        layout->first_column->right = new_root;
        layout->first_column->node->win.width /= 2;

        new_root->left                 = layout->first_column;
        new_root->right                = NULL;
        new_root->node->next           = NULL;
        new_root->node->previous       = NULL;
        new_root->node->win.window_id  = window_to_add_id;
        new_root->node->win.width      = layout->first_column->node->win.width;
        new_root->node->win.height     = layout->monitor_height;
        new_root->node->win.y_position = 0;
        new_root->node->win.x_position = layout->first_column->node->win.width;
}

static void
_add_window(struct amp_wm_layout* layout, unsigned long focused_window, unsigned long window_to_add_id)
{
        /* TODO */
        return;
}
