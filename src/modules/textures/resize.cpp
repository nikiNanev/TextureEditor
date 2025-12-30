#include "textures/resize.h"

void resize::apply(loader *loader, sdl_state *sdl_vstate, float &width, float &height)
{
    loader->texture->w = width;
    loader->texture->h = height;

    loader->width = width;
    loader->height = height;

    sdl_vstate->src.w = width;
    sdl_vstate->src.h = height;

    sdl_vstate->dst.w = width;
    sdl_vstate->dst.h = height;
}