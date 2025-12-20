#include "filters/exposure_adjustment.h"

static unsigned char clamp(int value)
{
    return static_cast<unsigned char>(value < 0 ? 0 : (value > 255 ? 255 : value));
}

bool exposure_adjustment::load(const std::string &filename, loader &loader)
{
    return loader.image_load(filename.c_str(), pixels_data);
}

bool exposure_adjustment::apply(float &exposure_factor, loader &loader, sdl_state *sdl_pstate)
{
    int pixel_index{0};

    int width = loader.width;
    int height = loader.height;
    int channels = loader.channels;

        if (channels >= 3)
    {
        for (int i = 0; i < height; ++i)
        {
            for (int j = 0; j < width; ++j)
            {
                pixel_index = (i * width + j) * channels;

                pixels_data[pixel_index + 0] = clamp(pixels_data[pixel_index + 0] * exposure_factor);
                pixels_data[pixel_index + 1] = clamp(pixels_data[pixel_index + 1] * exposure_factor);
                pixels_data[pixel_index + 2] = clamp(pixels_data[pixel_index + 2] * exposure_factor);
            }
        }
    }

    static int counter = 0;

    counter++;
    exporter exporter;
    std::string filename = exporter.formater("export_exposure_adjustment_", &counter, ".png");

    if (stbi_write_png(filename.c_str(), width, height, channels, pixels_data.data(), width * channels))
    {
        loader.texture_load(filename.c_str(), sdl_pstate->renderer, &sdl_pstate->src);
        return true;
    }
    return false;
}