#include "textures/exporter.h"
#include "textures/loader.h"
#include "states/editor_state.h"

typedef struct _posterization
{
    std::vector<unsigned char> pixels_data;

    bool load(const std::string &filename, loader &loader);
    bool apply(const uint8_t &levels, loader &loader, sdl_state *sdl_pstate);
} posterization;
