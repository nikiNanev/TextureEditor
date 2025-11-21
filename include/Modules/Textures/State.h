
// Main texture for editing

SDL_FRect src_texture = {
    .x = 0.0f,
    .y = 0.0f,
    .w = 0.0f,
    .h = 0.0f};

SDL_FRect dst_texture = {
    .x = 400.0f,
    .y = 200.0f,
    .w = 400.0f,
    .h = 400.0f};

bool is_dragging(SDL_FRect *dst_rect, const float *mouse_x, const float *mouse_y, float *drag_offset_x, float *drag_offset_y)
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