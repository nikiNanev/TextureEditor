#include "Modules/Textures/Loader.h"

#include <dlib/image_io.h>

SDL_Texture *Loader::texture_load(const char *filename, SDL_Renderer *renderer, SDL_FRect *src)
{
    texture = IMG_LoadTexture(renderer, filename);

    this->filename_path = filename;


    if (!texture)
    {
        std::cout << "Failed to create texture: " << SDL_GetError() << std::endl;
        return NULL;
    }

    src->w = texture->w;
    src->h = texture->h;

    this->is_texture = true;

    return texture;
}

void Loader::cleanup()
{
    SDL_DestroyTexture(texture);
}