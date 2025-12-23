#include "filters/borders.h"
#include "logger/profiler.h"

bool borders::load(const std::string &filename, loader &loader)
{
    return loader.image_load(filename.c_str(), pixels_data);
}

bool borders::apply(loader &loader, sdl_state *sdl_pstate, ImVec4 &color, int &width_border_pixels)
{
    // Profile the time consumption in the function
    profiler p;
    p.function = "Borders";

    int pixel_index{0};

    int width = loader.width;
    int height = loader.height;
    int channels = loader.channels;

    p.start = p.start_timer();
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            if (j >= 0 && j <= width_border_pixels || i >= 0 && i <= width_border_pixels ||
                j >= (width - width_border_pixels) && j <= width || i >= (height - width_border_pixels) && i <= height)
            {
                int idx = (i * width + j) * channels;

                pixels_data[idx + 0] = color.x * 256.f;
                pixels_data[idx + 1] = color.y * 256.f;
                pixels_data[idx + 2] = color.z * 256.f;
            }
        }
    }
    p.end = p.end_timer();

    static int counter = 0;

    counter++;

    p.report("report_borders_" + std::to_string(counter) + ".txt");

    exporter exporter;
    std::string filename = exporter.formater("export_borders_", &counter, ".png");

    if (stbi_write_png(filename.c_str(), width, height, channels, pixels_data.data(), width * channels))
    {
        loader.texture_load(filename.c_str(), sdl_pstate->renderer, &sdl_pstate->src);
        return true;
    }
    return false;
}