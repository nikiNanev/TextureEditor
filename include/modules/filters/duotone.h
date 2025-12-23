#include "textures/exporter.h"
#include "textures/loader.h"
#include "states/editor_state.h"
#include "logger/profiler.h"

typedef struct
{
    unsigned char r, g, b;
} color;

typedef struct _duotone
{
    std::vector<unsigned char> pixels_data;

    bool load(const std::string &filename, loader &loader);
    bool apply(color &shadow, color &highlight, loader &loader, sdl_state *sdl_pstate);
} duotone;
