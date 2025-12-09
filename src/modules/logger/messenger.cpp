#include "logger/messenger.h"

void message_state::display(sdl_state *sdl_vstate, imgui_state *imgui_vstate)
{
    if (init)
    {

        imgui_vstate->init_window_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration;

        current_time = SDL_GetTicks();

        if (alpha >= 1.0f)
        {
            alpha = 1.0f;

            is_showed = true;
        }
        else if (!is_showed)
        {
            if (current_time > last_time + 30.0f)
            {
                alpha += 0.05f;
                last_time = current_time
                ;
            }
        }

        if (is_showed)
        {

            if (current_time > last_time + 1000.0f)
            {
                seconds++;
                last_time = current_time;
            }

            if (seconds >= 2)
            {
                if (alpha <= 0.0f)
                {
                    // Reset
                    alpha = 0.0f;
                    is_showed = false;
                    init = false;
                    seconds = 0;
                }
                else if (current_time > last_time + 30.0f)
                {
                    // Fade Out
                    alpha -= 0.05f;
                    last_time = current_time;
                }
            }
        }

        ImGuiStyle &style = ImGui::GetStyle();

        float sizet = ImGui::CalcTextSize(message.c_str()).x + style.FramePadding.x * 2.0f;
        float avail = ImGui::GetContentRegionAvail().x;
        float alignment = 0.5f;
        float off = (avail - sizet) * alignment;

        size.x = avail;
        size.y = 10.0f;

        ImGui::SetNextWindowPos(pos);
        ImGui::SetNextWindowSize(size);
        ImGui::SetNextWindowBgAlpha(alpha);
        ImGui::Begin("[INFO]", NULL, imgui_vstate->init_window_flags);

        if (off > 0.0f)
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);

        ImGui::TextColored(color, "%s", message.c_str());

        ImGui::End();
    }
}