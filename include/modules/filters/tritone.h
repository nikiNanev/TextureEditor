#include "textures/exporter.h"
#include "textures/loader.h"
#include "states/editor_state.h"

#pragma once
#include "duotone.h"

typedef struct _tritone
{
    std::vector<unsigned char> pixels_data;

    bool load(const std::string &filename, loader &loader);
    bool apply(color &shadow, color &mid, color &highlight, loader &loader, sdl_state *sdl_pstate);
} tritone;
