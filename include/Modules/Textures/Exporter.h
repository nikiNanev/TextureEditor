#pragma once

#include "common.h"

class Exporter
{

private:
    bool is_exported = false;
    std::string filename;

public:
    Exporter(std::string filename) : filename(filename){}

    bool toPNG(SDL_Texture *texture, SDL_Surface *surface);
    bool toJPEG(SDL_Texture *texture, SDL_Surface *surface);
    bool toBMP(SDL_Texture *texture, SDL_Surface *surface);

}; 