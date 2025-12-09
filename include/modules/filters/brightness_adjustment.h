#include "textures/exporter.h"
#include "textures/loader.h"
#include "states/editor_state.h"

typedef struct _brightness_adjustment
{
    float alpha;
    int beta;

    std::vector<unsigned char> pixel_data;

    bool load(const std::string &filename, loader &loader);
    bool apply(const float &alpha, const int &beta, loader &loader, sdl_state *sdl_pstate);
} brightness_adjustment;
