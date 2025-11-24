#pragma once
#include "EditorState.h"

typedef struct message_state
{
    std::string message;
    int length{static_cast<int>(message.length())};

    ImVec2 size;
    const ImVec2 pos{20.0f, 30.0f};
    const ImVec4 color{0.0f, 0.7f, 0.0f, 1.00f};

    float alpha = 0.0f;

    bool is_showed{false};
    bool init{false};

    // Timing is everything
    float lastTime = 0.0f, currentTime = 0.0f;
    int seconds = 0;

} message_state;

class Messenger
{
public:
    Messenger() {}

    void display(sdl_state *sdl_vstate, imgui_state *imgui_vstate, message_state *message_vstate);
};