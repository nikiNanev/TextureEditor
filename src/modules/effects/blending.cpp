#include "effects/blending.h"
#include "logger/profiler.h"

bool blending::load(const std::string &filename, loader &loader)
{
    return loader.image_load(filename.c_str(), loader.pixels_data);
}

bool blending::apply(loader &loader, sdl_state &sdl_pstate)
{
    // Profile the time consumption in the function
    profiler p;
    p.function = "Blending";

    p.start = p.start_timer();

    idx = 0;

    std::cout << "Channels: " << loader.channels << std::endl;
    std::cout << "Width: " << width << " Height: " << height << std::endl;
    std::cout << "Width: " << (int)width << " Height: " << (int)height << std::endl;

    for (int i = 0; i < loader.width; i++)
    {
        for (int j = 0; j < loader.height; j++)
        {
            idx = (i * loader.height + j) * loader.channels;

            if (i <= width && j <= height)
            {
                loader.pixels_data[idx + 3] = alpha;
            }
        }
    }

    p.end = p.end_timer();

    static int counter = 0;

    counter++;

    p.report("report_blending_" + std::to_string(counter) + ".txt");

    exporter exporter;
    std::string filename = exporter.formater("export_blending_", &counter, ".png");

    if (stbi_write_png(filename.c_str(), loader.width, loader.height, loader.channels, loader.pixels_data.data(), loader.width * loader.channels))
    {
        loader.texture_load(filename.c_str(), sdl_pstate.renderer, &sdl_pstate.src);
        return true;
    }
    return false;
}