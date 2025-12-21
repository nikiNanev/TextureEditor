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
    bool borders{false};
    bool colorize{false};

    bool edge_enhancement{false};
    bool high_pass{false};
    bool brightness_adjustment{false};
    bool contrast_adjustment{false};
    bool exposure_adjustment{false};

    bool binary_thresholds{false};
    bool film_grain{false};

    bool emboss{false};
    bool gamma_correction{false};
    bool invert{false};
    bool grayscale{false};
    bool posterization{false};

    bool duotone{false};
    bool tritone{false};
    
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

    bool is_flip{false};
    bool is_rotate{false};

    float scale{1.0f};
} edit;

typedef struct styles
{
    bool theme{false};
    bool background{false};
} styles;

typedef struct info
{
    bool desc{false};
} info;

typedef struct report_state
{
    bool init{false};
} report_state;

typedef struct _help
{
    bool manual{false};
} help;

typedef struct image_state
{
    bool general_info{false};

} image_state;

typedef struct _editor_state
{
    export_state export_state;
    report_state report_state;
    image_state image_state;

    info info;
    filters filter;
    edit edit;

    styles styles;
    help help;

    bool is_processing{false};
} editor_state;
