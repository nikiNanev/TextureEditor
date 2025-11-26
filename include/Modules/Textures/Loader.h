#pragma once

#include "common.h"

class Loader
{
    SDL_Texture *texture{NULL};

    bool is_texture{false};

    std::string filename_path;

public:
    Loader()
    {
    }

    SDL_Texture *texture_load(const char *filePath, SDL_Renderer *renderer, SDL_FRect *src);

    void set_texture(SDL_Texture *texture)
    {
        this->texture = texture;
    }

    SDL_Texture *get_texture() const
    {
        return this->texture;
    }

    bool is_texture_loaded()
    {
        return this->is_texture;
    }

    void set_filename_path(const std::string filename_path)
    {
        this->filename_path = filename_path;
    }

    std::string get_filename_path()
    {
        return this->filename_path;
    }

    void cleanup();
};