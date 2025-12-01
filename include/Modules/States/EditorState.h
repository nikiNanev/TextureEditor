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
    bool brightness_adjustment{false};
} filters;

typedef struct _export_state
{
    bool open_modal{false};

    bool is_exported{false};

} export_state;

typedef struct edit
{
    bool is_resize{false};
    bool is_scale{false};

    float scale{1.0f};
} edit;

typedef struct Styles
{
    bool theme{false};
    bool background{false};
} Styles;

typedef struct Info
{
    bool desc{false};
} Info;

typedef struct report
{
    bool init{false};
} Report_state;

typedef struct _help
{
    bool manual{false};
} help;

typedef struct _editor_state
{
    // Export Options
    export_state export_st;

    Info info;

    // Filters
    filters filter;

    edit edit;

    Styles styles;

    Report_state report;

    help help;

    bool is_processing{false};
} editor_state;
