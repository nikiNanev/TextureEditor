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

SDL_Texture *loader::create_texture(color &color, const char *filename, int width, int height, sdl_state &sdl_vstate)
{
    this->is_texture = true;
    this->channels = 4;

    int pixel_index{0};

    this->pixels_data = std::vector<unsigned char>(width * height * channels);

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            pixel_index = (i * height + j) * channels;

            pixels_data[pixel_index + 0] = color.r;
            pixels_data[pixel_index + 1] = color.g;
            pixels_data[pixel_index + 2] = color.b;
            pixels_data[pixel_index + 3] = 255;
        }
    }

    if (strlen(filename) > 3 && strlen(filename) < 50)
    {
        this->filename_path = "../assets/images/";
        this->filename_path.append(filename);
        this->filename_path.append(".png");
    }
    else
    {
        this->filename_path = "../assets/images/new_texture.png";
    }

    if (stbi_write_png(this->filename_path.c_str(), width, height, this->channels, pixels_data.data(), width * this->channels))
    {
        texture_load(this->filename_path.c_str(), sdl_vstate.renderer, &sdl_vstate.src);
    }

    sdl_vstate.src.w = texture->w;
    sdl_vstate.src.h = texture->h;

    sdl_vstate.dst.w = texture->w;
    sdl_vstate.dst.h = texture->h;

    return texture;
}

void loader::cleanup()
{
    SDL_DestroyTexture(texture);
}