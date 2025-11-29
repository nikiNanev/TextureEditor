#include "Modules/Textures/Resize.h"

void Resize::apply(Loader *loader, sdl_state *sdl_vstate, float &width, float &height)
{
    loader->texture->w = width;
    loader->texture->h = height;

    sdl_vstate->src.w = width;
    sdl_vstate->src.h = height;

    sdl_vstate->dst.w = width;
    sdl_vstate->dst.h = height;

    
}