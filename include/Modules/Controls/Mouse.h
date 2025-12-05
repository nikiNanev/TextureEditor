#pragma once
#include "Modules/States/EditorState.h"

typedef struct _controls
{
    bool is_texture_dragging{false};
    float drag_offset_x, drag_offset_y;
} mouse_controls;

class Mouse
{

public:
    Mouse() {
    }

    void dragging(SDL_Event *event, sdl_state &sdl_vstate, mouse_controls *mouse_pcontrols, SDL_FRect &rect, SDL_FPoint &point, bool &is_processing);
    bool is_dragging(SDL_FRect *dst_rect, const float *mouse_x, const float *mouse_y, float *drag_offset_x, float *drag_offset_y);
};