#include "Loader.h"
#include "Modules/States/EditorState.h"

typedef struct Resize
{
    Resize() {}
    void apply(Loader *loader, sdl_state *sdl_vstate, float &width, float &height);
} Resize;