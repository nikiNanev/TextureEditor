#pragma once
#include "common.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#endif

#include "states/editor_state.h"
#include "utilities/color.h"

typedef struct _loader
{

    int width, height, channels;

    size_t size{0};
    std::vector<unsigned char> pixels_data;
    SDL_Texture *texture{NULL};

    bool is_texture{false};

    std::string filename_path;

    SDL_Texture *create_texture(color &color, std::string filename, int width, int height, sdl_state &sdl_vstate);
    SDL_Texture *texture_load(const char *filePath, SDL_Renderer *renderer, SDL_FRect *src);
    bool image_load(const char *filename, std::vector<unsigned char> &pixels_data);
    void cleanup();
} loader;