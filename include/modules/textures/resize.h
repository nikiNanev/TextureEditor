#include "loader.h"
#include "states/editor_state.h"

typedef struct _resize
{
    void apply(loader *loader, sdl_state *sdl_vstate, float &width, float &height);
} resize;