#include "textures/loader.h"

SDL_Texture *loader::texture_load(const char *filename, SDL_Renderer *renderer, SDL_FRect *src)
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

bool loader::image_load(const char *filename, std::vector<unsigned char> &pixels_data)
{
    uint8_t desired_channels = 4;

    unsigned char *loaded_data = stbi_load(
        filename,
        &width,
        &height,
        &channels,
        desired_channels);

    if (loaded_data)
    {
        // desired_channels is 0 if we kept original, otherwise it's the forced value
        channels = desired_channels != 0 ? desired_channels : channels;

        size_t size = static_cast<size_t>(width) * height * channels;
        pixels_data.assign(loaded_data, loaded_data + size);

        // Free the memory allocated by stb_image
        stbi_image_free(loaded_data);
        return true;
    }
    else
    {
        std::cout << "Failed to load image: " + std::string(filename) + " - " + stbi_failure_reason() << std::endl;
        return false;
    }
}

void loader::cleanup()
{
    SDL_DestroyTexture(texture);
}