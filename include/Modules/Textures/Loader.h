#pragma once

#include "common.h"

class Loader
{
    SDL_Surface *surface{NULL};
    SDL_Texture *texture{NULL};
public:
    Loader()
    {
    }
    SDL_Texture *texture_load(const char *filePath, SDL_Renderer *renderer, SDL_FRect *src);

    SDL_Surface *get_surface()
    {
        return this->surface;
    }

    SDL_Texture *get_texture()
    {
        return this->texture;
    }

    void cleanup();
};