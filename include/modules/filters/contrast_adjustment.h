#include "textures/exporter.h"
#include "textures/loader.h"
#include "states/editor_state.h"

typedef struct _contrast_adjustment
{
    std::vector<unsigned char> pixels_data;

    bool load(const std::string &filename, loader &loader);
    bool apply(const float &contrast_factor, loader &loader, sdl_state *sdl_pstate);
} contrast_adjustment;
