#include "textures/exporter.h"
#include "textures/loader.h"
#include "states/editor_state.h"

typedef struct _blending
{
    float width, height;

    unsigned char alpha;

    long idx;

    bool load(const std::string &filename, loader &loader);
    bool apply(loader &loader, sdl_state &sdl_pstate);

} blending;