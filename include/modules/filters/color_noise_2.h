#pragma once

#include "textures/exporter.h"
#include "textures/loader.h"
#include "states/editor_state.h"
#include "utilities/color.h"

typedef struct _color_noise_2
{
    std::vector<unsigned char> pixels_data;
    float intensity1{0.0f};
    float intensity2{0.0f};
    
    color color1;
    color color2;

    bool load(const std::string &filename, loader &loader);
    bool apply(loader &loader, sdl_state *sdl_pstate);
} color_noise_2;