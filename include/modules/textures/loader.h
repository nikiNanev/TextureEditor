#pragma once
#include "common.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#endif

typedef struct _loader
{

    int width, height, channels;

    SDL_Texture *texture{NULL};

    bool is_texture{false};

    std::string filename_path;
    SDL_Texture *texture_load(const char *filePath, SDL_Renderer *renderer, SDL_FRect *src);
    bool image_load(const char *filename, std::vector<unsigned char> &pixels_data);
    void cleanup();
} loader;