#pragma once

#include "textures/exporter.h"
#include "textures/loader.h"
#include "states/editor_state.h"
#include "utilities/color.h"

typedef struct _color_noise
{
    std::vector<unsigned char> pixels_data;
    float intensity{0.0f};
    color color;

    bool load(const std::string &filename, loader &loader);
    bool apply(loader &loader, sdl_state *sdl_pstate);
} color_noise;