typedef struct _rotations
{

    void controls(SDL_Event &event, double &angle, double &factor_angle, double &add_wait_seconds, message_state &message_vstate)
    {
        if (event.key.key == SDLK_R)
        {
            angle += factor_angle;

            message_vstate.init = true;
            message_vstate.message = "Rotated " + (std::ostringstream() << std::fixed << std::setprecision(2) << angle).str();

            message_vstate.seconds -= add_wait_seconds;
        }

        if (event.key.key == SDLK_T)
        {
            angle -= factor_angle;

            message_vstate.init = true;
            message_vstate.message = "Rotated " + (std::ostringstream() << std::fixed << std::setprecision(2) << angle).str();

            message_vstate.seconds -= add_wait_seconds;
        }

        if (event.key.key == SDLK_F)
        {
            factor_angle += 0.5f;

            message_vstate.init = true;
            message_vstate.message = "Factor angle " + (std::ostringstream() << std::fixed << std::setprecision(2) << factor_angle).str();

            message_vstate.seconds -= add_wait_seconds;
        }

        if (event.key.key == SDLK_D)
        {
            factor_angle -= 0.5f;

            message_vstate.init = true;
            message_vstate.message = "Factor angle " + (std::ostringstream() << std::fixed << std::setprecision(2) << factor_angle).str();

            message_vstate.seconds -= add_wait_seconds;
        }
    }

} rotations;