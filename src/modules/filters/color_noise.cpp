#include "filters/color_noise.h"
#include "logger/profiler.h"

bool color_noise::load(const std::string &filename, loader &loader)
{
    return loader.image_load(filename.c_str(), pixels_data);
}

bool color_noise::apply(loader &loader, sdl_state *sdl_pstate)
{
    // Profile the time consumption in the function
    profiler p;
    p.function = "Color Noise";

    std::srand(std::time(nullptr));

    int pixel_index{0};

    int width = loader.width;
    int height = loader.height;
    int channels = loader.channels;

    p.start = p.start_timer();
    
    if (channels >= 3)
    {
        for (int i = 0; i < width; ++i)
        {
            for (int j = 0; j < height; ++j)
            {
                pixel_index = (i * height + j) * channels;

                if((double)rand() / RAND_MAX < (intensity / 100))// 0 ----- 1
                {
                    pixels_data[pixel_index + 0] = color.r;
                    pixels_data[pixel_index + 1] = color.g;
                    pixels_data[pixel_index + 2] = color.b;
                }
            }
        }
    }

    p.end = p.end_timer();

    static int counter = 0;

    counter++;
    p.report("report_color_noise_" + std::to_string(counter) + ".txt");
    exporter exporter;
    std::string filename = exporter.formater("export_color_noise_", &counter, ".png");

    if (stbi_write_png(filename.c_str(), width, height, channels, pixels_data.data(), width * channels))
    {
        loader.texture_load(filename.c_str(), sdl_pstate->renderer, &sdl_pstate->src);
        return true;
    }
    return false;
}