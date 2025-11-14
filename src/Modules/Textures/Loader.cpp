#include "Modules/Textures/Loader.h"

SDL_Texture *Loader::texture_load(const char *filePath, SDL_Renderer *renderer, SDL_FRect *src)
{
    surface = IMG_Load(filePath);

    if (!surface)
    {
        std::cout << "Failed to load image: " << SDL_GetError() << std::endl;
        return NULL;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture)
    {
        std::cout << "Failed to create texture: " << SDL_GetError() << std::endl;
        return NULL;
    }

    src->w = texture->w;
    src->h = texture->h;

    return texture;
}

void Loader::cleanup()
{
    SDL_DestroySurface(surface);
    SDL_DestroyTexture(texture);
}