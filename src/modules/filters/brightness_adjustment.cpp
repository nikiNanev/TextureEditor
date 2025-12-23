#include "filters/brightness_adjustment.h"
#include "logger/profiler.h"

bool brightness_adjustment::load(const std::string &filename, loader &loader)
{
    return loader.image_load(filename.c_str(), pixels_data);
}

bool brightness_adjustment::apply(const float &alpha, const int &beta, loader &loader, sdl_state *sdl_pstate)
{
    // Profile the time consumption in the function
    profiler p;
    p.function = "Brightness Adjustment";

    static int counter = 0;

    int pixel_index{0};

    int width = loader.width;
    int height = loader.height;
    int channels = loader.channels;

    p.start = p.start_timer();

    if (channels >= 3)
    {
        for (int i = 0; i < loader.width; ++i)
        {
            for (int j = 0; j < loader.height; ++j)
            {
                pixel_index = (i * height + j) * channels;

                pixels_data[pixel_index + 0] = (unsigned char)(alpha * pixels_data[pixel_index + 0] + beta);
                pixels_data[pixel_index + 1] = (unsigned char)(alpha * pixels_data[pixel_index + 1] + beta);
                pixels_data[pixel_index + 2] = (unsigned char)(alpha * pixels_data[pixel_index + 2] + beta);
            }
        }
    }
    p.end = p.end_timer();

    counter++;
    p.report("report_brightness_adjustment_" + std::to_string(counter) + ".txt");

    exporter exporter;
    std::string filename = exporter.formater("export_brightness_adjustment_", &counter, ".png");

    stbi_write_png(filename.c_str(), width, height, channels, pixels_data.data(), width * channels);

    loader.texture_load(filename.c_str(), sdl_pstate->renderer, &sdl_pstate->src);

    return true;
}