#include "filters/roberts_cross.h"
#include "logger/profiler.h"

static unsigned char clamp(int v)
{
    return (unsigned char)(v < 0 ? 0 : (v > 255 ? 255 : v));
}

bool roberts_cross::load(const std::string &filename, loader &loader)
{
    return loader.image_load(filename.c_str(), loader.pixels_data);
}

bool roberts_cross::apply(loader &loader, sdl_state *sdl_pstate)
{
    // Profile the time consumption in the function
    profiler p;
    p.function = "Roberts cross";

    size_t size = loader.width * loader.height * loader.channels;

    std::vector<unsigned char> out(size);

    // Apply Roberts Cross (skip borders)
    for (int y = 0; y < loader.height - 1; ++y) {
        for (int x = 0; x < loader.width - 1; ++x) {
            int i = y * loader.width + x;

            int p00 = loader.pixels_data[i];
            int p01 = loader.pixels_data[i + 1];
            int p10 = loader.pixels_data[i + loader.width];
            int p11 = loader.pixels_data[i + loader.width + 1];

            int gx = p00 - p11;
            int gy = p01 - p10;
            
            int mag = abs(gx) + abs(gy);

            out[i] = clamp(mag);
        }

        // Last column
        out[y * loader.width + (loader.width - 1)] = 0;
    }

    // Last row
    for (int x = 0; x < loader.width; ++x) out[(loader.height - 1) * loader.width + x] = 0;

    p.end = p.end_timer();

    static int counter = 0;

    counter++;
    p.report("report_roberts_cross_" + std::to_string(counter) + ".txt");
    exporter exporter;
    std::string filename = exporter.formater("export_roberts_cross_", &counter, ".png");

    if (stbi_write_png(filename.c_str(), loader.width, loader.height, loader.channels, out.data(), loader.width * loader.channels))
    {
        loader.texture_load(filename.c_str(), sdl_pstate->renderer, &sdl_pstate->src);
        return true;
    }
    return false;
}