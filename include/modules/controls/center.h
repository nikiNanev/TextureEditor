typedef struct _center
{
    SDL_FRect rect;
    SDL_FPoint point;
    double angle;

    void init(sdl_state *sdl_vstate)
    {
        point = {
            .x = 0.0f,
            .y = 0.0f
        };

        rect = {
            .x = sdl_vstate->dst.x,
            .y = sdl_vstate->dst.y,
            .w = 10.0f,
            .h = 10.0f};
    }

    void controls(SDL_Event &event, message_state &message_vstate, SDL_FPoint *point, SDL_Texture *texture, sdl_state *sdl_vstate)
    {
        if (event.key.key == SDLK_1)
        {
            point->x += 0.5f;

            message_vstate.init = true;
            message_vstate.message = "Center x: " + (std::ostringstream() << std::fixed << std::setprecision(2) << point->x).str();
        }

        if (event.key.key == SDLK_2)
        {
            point->y += 0.5f;
            
            message_vstate.init = true;
            message_vstate.message = "Center y: " + (std::ostringstream() << std::fixed << std::setprecision(2) << point->y).str();
        }

        if (event.key.key == SDLK_3)
        {
            point->x -= 0.5f;

            message_vstate.init = true;
            message_vstate.message = "Center x: " + (std::ostringstream() << std::fixed << std::setprecision(2) << point->x).str();
        }

        if (event.key.key == SDLK_4)
        {
            point->y -= 0.5f;

            message_vstate.init = true;
            message_vstate.message = "Center y: " + (std::ostringstream() << std::fixed << std::setprecision(2) << point->y).str();
        }

        rect = {
            .x = sdl_vstate->dst.x + point->x,
            .y = sdl_vstate->dst.y + point->y,
            .w = 10.0f,
            .h = 10.0f};

        
    }
} center;