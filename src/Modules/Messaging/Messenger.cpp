#include "Modules/Messaging/Messenger.h"

void Messenger::display(sdl_state *sdl_vstate, imgui_state *imgui_vstate, message_state *message_vstate)
{
    if (message_vstate->init)
    {

        imgui_vstate->init_window_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration;

        message_vstate->currentTime = SDL_GetTicks();

        if (message_vstate->alpha >= 1.0f)
        {
            message_vstate->alpha = 1.0f;

            message_vstate->is_showed = true;
        }
        else if (!message_vstate->is_showed)
        {
            if (message_vstate->currentTime > message_vstate->lastTime + 30.0f)
            {
                message_vstate->alpha += 0.05f;
                message_vstate->lastTime = message_vstate->currentTime;
            }
        }

        if (message_vstate->is_showed)
        {

            if (message_vstate->currentTime > message_vstate->lastTime + 1000.0f)
            {
                message_vstate->seconds++;
                message_vstate->lastTime = message_vstate->currentTime;
            }

            if (message_vstate->seconds == 2)
            {
                if (message_vstate->alpha <= 0.0f)
                {
                    // Reset
                    message_vstate->alpha = 0.0f;
                    message_vstate->is_showed = false;
                    message_vstate->init = false;
                    message_vstate->seconds = 0;
                }
                else if (message_vstate->currentTime > message_vstate->lastTime + 30.0f)
                {
                    // Fade Out
                    message_vstate->alpha -= 0.05f;
                    message_vstate->lastTime = message_vstate->currentTime;
                }
            }
        }

        ImGuiStyle &style = ImGui::GetStyle();

        float sizet = ImGui::CalcTextSize(message_vstate->message.c_str()).x + style.FramePadding.x * 2.0f;
        float avail = ImGui::GetContentRegionAvail().x;
        float alignment = 0.5f;
        float off = (avail - sizet) * alignment;

        message_vstate->size.x = avail;
        message_vstate->size.y = 10.0f;

        ImGui::SetNextWindowPos(message_vstate->pos);
        ImGui::SetNextWindowSize(message_vstate->size);
        ImGui::SetNextWindowBgAlpha(message_vstate->alpha);
        ImGui::Begin("[INFO]", NULL, imgui_vstate->init_window_flags);

        if (off > 0.0f)
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);

        ImGui::TextColored(message_vstate->color, "%s", message_vstate->message.c_str());

        ImGui::End();
    }
}