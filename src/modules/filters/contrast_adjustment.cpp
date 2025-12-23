#include "filters/contrast_adjustment.h"
#include "logger/profiler.h"

bool contrast_adjustment::load(const std::string &filename, loader &loader)
{
    return loader.image_load(filename.c_str(), pixels_data);
}

static unsigned char clamp(int value)
{
    return static_cast<unsigned char>(value < 0 ? 0 : (value > 255 ? 255 : value));
}

bool contrast_adjustment::apply(const float &contrast_factor, loader &loader, sdl_state *sdl_pstate)
{
    // Profiling the time consumption in the function
    profiler p;
    p.function = "Contrast Adjustment";

    p.start = p.start_timer();

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

                pixels_data[pixel_index + 0] = clamp(static_cast<int>(contrast_factor * (pixels_data[pixel_index + 0] - 128) + 128));
                pixels_data[pixel_index + 1] = clamp(static_cast<int>(contrast_factor * (pixels_data[pixel_index + 1] - 128) + 128));
                pixels_data[pixel_index + 2] = clamp(static_cast<int>(contrast_factor * (pixels_data[pixel_index + 2] - 128) + 128));
            }
        }
    }

    p.end = p.end_timer();

    counter++;
    p.report("report_constrast_adjustment_" + std::to_string(counter) + ".txt");

    exporter exporter;
    std::string filename = exporter.formater("export_contrast_adjustment_", &counter, ".png");

    stbi_write_png(filename.c_str(), width, height, channels, pixels_data.data(), width * channels);

    loader.texture_load(filename.c_str(), sdl_pstate->renderer, &sdl_pstate->src);

    return true;
}