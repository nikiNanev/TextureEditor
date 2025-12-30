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

        size = static_cast<size_t>(width) * height * channels;
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

SDL_Texture *loader::create_texture(color &color, const char *filename, int &width, int &height, SDL_Renderer *renderer)
{
    SDL_Surface *surface = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_RGBA8888);
    const SDL_PixelFormatDetails *details = SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA8888);
    std::cout << "color ( r: " << (int)color.r << " g: " << (int)color.g << " b: " << (int)color.b << std::endl;
    SDL_FillSurfaceRect(surface, NULL, SDL_MapRGBA(details, NULL, (uint8_t)color.r, (uint8_t)color.g, (uint8_t)color.b, 255)); 
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);

    if (texture == NULL)
    {
        SDL_Log("Failed to create texture: %s", SDL_GetError());
        return NULL;
    }
    this->is_texture = true;
    return texture;
}

void loader::cleanup()
{
    SDL_DestroyTexture(texture);
}