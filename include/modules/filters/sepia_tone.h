#pragma once
#include "textures/exporter.h"
#include "textures/loader.h"
#include "states/editor_state.h"
#include "utilities/color.h"

typedef struct _sepia_tone
{
    std::vector<unsigned char> pixels_data;
    color color;

    bool load(const std::string &filename, loader &loader);
    bool apply(loader &loader, sdl_state *sdl_pstate);
} sepia_tone;