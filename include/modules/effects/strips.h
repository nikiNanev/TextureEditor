#include "textures/exporter.h"
#include "textures/loader.h"
#include "states/editor_state.h"

typedef struct _strips
{
    std::vector<unsigned char> image;

    int horizontal_strips, vertical_strips;
    color color;

    long idx;

    bool load(const std::string &filename, loader &loader);
    bool apply(loader &loader, sdl_state &sdl_pstate);

} strips;