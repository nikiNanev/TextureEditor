#include "Modules/Textures/Loader.h"

#include <dlib/image_io.h>

SDL_Texture *Loader::texture_load(const char *filePath, SDL_Renderer *renderer, SDL_FRect *src)
{
    surface = IMG_Load(filePath);

    // try
    // {
    //     const std::string file = filePath;
    //     std::cout << "Path: " << file << std::endl;
    //     dlib::array2d<dlib::rgb_pixel> image;
    //     dlib::load_image(image, file);

    //     std::cout << "Size of the array2d: " << sizeof(image) << " bytes " << std::endl;

    //     const std::string file_output = "export.jpeg";
    //     dlib::save_jpeg(image, file_output, 95);

    // }
    // catch (const std::exception &e)
    // {
    //     std::cerr << e.what() << '\n';
    // }

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