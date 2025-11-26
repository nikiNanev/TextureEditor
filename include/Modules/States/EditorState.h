#pragma once

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

#include "imfilebrowser.h"

#include "common.h"

typedef struct sdl_state
{

    SDL_Window *window;
    SDL_Renderer *renderer;
    int width, height;

    SDL_FRect src;
    SDL_FRect dst;
} sdl_state;

typedef struct imgui_state
{
    int init_window_flags;
    ImVec4 clear_color;
} imgui_state;

// Filters
typedef struct filters
{
    bool blur{false};
    bool edge_enhancement{false};
} filters;

typedef struct _export_state
{
    bool open_modal{false};

    bool is_exported{false};

} export_state;

typedef struct _editor_state
{
    // Export Options
    export_state export_st;

    // Filters
    filters filter;

    bool is_processing{false};

} editor_state;
