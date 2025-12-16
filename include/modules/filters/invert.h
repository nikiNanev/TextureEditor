#pragma once

#include "textures/exporter.h"
#include "textures/loader.h"
#include "states/editor_state.h"

typedef struct _invert
{
    std::vector<unsigned char> pixels_data;

    float rgb[3];

    bool load(const std::string &filename, loader &loader);
    bool apply(loader &loader, sdl_state *sdl_pstate);
} invert;