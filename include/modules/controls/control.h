typedef struct _control
{
    void activate_resize(SDL_Event &event, message_state &message_vstate, editor_state &editor_vstate)
    {
        if (event.key.key == SDLK_L)
        {
            editor_vstate.edit.is_resize = true;

            message_vstate.init = true;
            message_vstate.message = " Resize ";
        }
    }
} control;