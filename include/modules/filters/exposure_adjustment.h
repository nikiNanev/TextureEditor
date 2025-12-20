#pragma once

#include "textures/exporter.h"
#include "textures/loader.h"
#include "states/editor_state.h"

typedef struct _exposure_adjustment
{
    std::vector<unsigned char> pixels_data;

    bool load(const std::string &filename, loader &loader);
    bool apply(float &exposure_factor, loader &loader, sdl_state *sdl_pstate);
} exposure_adjustment;