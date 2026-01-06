#pragma once
#include "textures/exporter.h"
#include "textures/loader.h"
#include "states/editor_state.h"
#include "utilities/color.h"

typedef struct _roberts_cross
{
    bool load(const std::string &filename, loader &loader);
    bool apply(loader &loader, sdl_state *sdl_pstate);
} roberts_cross;