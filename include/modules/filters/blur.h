
#include "textures/exporter.h"
#include "textures/loader.h"
#include "states/editor_state.h"

typedef struct _blur
{
    std::vector<unsigned char> pixels_data;

    bool load(const std::string &filename, loader &loader);
    bool apply(const float &sigma, loader &loader, sdl_state *sdl_pstate);
} blur;