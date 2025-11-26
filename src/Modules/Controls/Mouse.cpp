#include "Modules/Controls/Mouse.h"

void Mouse::dragging(SDL_Event *event, sdl_state &sdl_vstate, mouse_controls *mouse_pcontrols, bool &is_processing)
{
    switch (event->type)
    {
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
        if (event->button.button == SDL_BUTTON_LEFT)
        {
            float mouse_x = event->button.x;
            float mouse_y = event->button.y;
            mouse_pcontrols->is_texture_dragging = !is_processing && is_dragging(&sdl_vstate.dst, &mouse_x, &mouse_y, &mouse_pcontrols->drag_offset_x, &mouse_pcontrols->drag_offset_y);
        }
        break;

    case SDL_EVENT_MOUSE_MOTION:
        if (mouse_pcontrols->is_texture_dragging)
        {
            // Update dst_rect position based on mouse motion
            sdl_vstate.dst.x = event->motion.x - mouse_pcontrols->drag_offset_x;
            sdl_vstate.dst.y = event->motion.y - mouse_pcontrols->drag_offset_y;
        }
        break;
    case SDL_EVENT_MOUSE_BUTTON_UP:
        if (event->button.button == SDL_BUTTON_LEFT)
        {
            mouse_pcontrols->is_texture_dragging = false; // Release the texture
        }
        break;
    }
}

bool Mouse::is_dragging(SDL_FRect *dst_rect, const float *mouse_x, const float *mouse_y, float *drag_offset_x, float *drag_offset_y)
{
    // Check if mouse click collides with the texture's dst_rect
    if (*mouse_x >= dst_rect->x && *mouse_x < dst_rect->x + dst_rect->w &&
        *mouse_y >= dst_rect->y && *mouse_y < dst_rect->y + dst_rect->h)
    {
        // Calculate offset to keep the drag relative to click position
        *drag_offset_x = *mouse_x - dst_rect->x;
        *drag_offset_y = *mouse_y - dst_rect->y;
        return true;
    }
    return false;
}