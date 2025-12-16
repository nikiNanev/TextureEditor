#include "filters/colorize.h"


bool colorize::load(const std::string &filename, loader &loader)
{
    return loader.image_load(filename.c_str(), pixels_data);
}

bool colorize::apply(loader &loader, sdl_state *sdl_pstate)
{

    int pixel_index{0};

    int width = loader.width;
    int height = loader.height;
    int channels = loader.channels;

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            pixel_index = (i * width + j) * channels;
            
            pixels_data[pixel_index + 0] = (rgb[0] * 256.f) - 128.f + pixels_data[pixel_index + 0];
            pixels_data[pixel_index + 1] = (rgb[1] * 256.f) - 128.f + pixels_data[pixel_index + 1];
            pixels_data[pixel_index + 2] = (rgb[2] * 256.f) - 128.f + pixels_data[pixel_index + 2];
        }
    }

    static int counter = 0;

    counter++;
    exporter exporter;
    std::string filename = exporter.formater("export_colorized_", &counter, ".png");

    if (stbi_write_png(filename.c_str(), width, height, channels, pixels_data.data(), width * channels))
    {
        loader.texture_load(filename.c_str(), sdl_pstate->renderer, &sdl_pstate->src);
        return true;
    }
    return false;
}