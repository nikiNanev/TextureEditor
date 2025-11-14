#include "Modules/Textures/Exporter.h"

bool Exporter::toPNG(SDL_Texture *texture, SDL_Surface *surface)
{
    if (texture)
    {
        this->is_exported = IMG_SavePNG(surface, this->filename.c_str());
        return this->is_exported;
    }

    return false;
}

bool Exporter::toJPEG(SDL_Texture *texture, SDL_Surface *surface)
{
    return true;
}

bool Exporter::toBMP(SDL_Texture *texture, SDL_Surface *surface)
{
    return true;
}