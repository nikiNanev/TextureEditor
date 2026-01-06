#include "filters/sepia_tone.h"
#include "logger/profiler.h"

static unsigned char clamp(int v)
{
    return (unsigned char)(v < 0 ? 0 : (v > 255 ? 255 : v));
}

bool sepia_tone::load(const std::string &filename, loader &loader)
{
    return loader.image_load(filename.c_str(), pixels_data);
}

bool sepia_tone::apply(loader &loader, sdl_state *sdl_pstate)
{
    // Profile the time consumption in the function
    profiler p;
    p.function = "Sepia tone";

    size_t size = loader.width * loader.height * loader.channels;

    for (int i = 0; i < size; i += 3)
    {
        color.r = pixels_data[i];
        color.g = pixels_data[i + 1];
        color.b = pixels_data[i + 2];

        // Sepia tone
        unsigned char new_r = clamp((int)(color.r * 0.393 + color.g * 0.769 + color.b * 0.189));
        unsigned char new_g = clamp((int)(color.r * 0.349 + color.g * 0.686 + color.b * 0.168));
        unsigned char new_b = clamp((int)(color.r * 0.272 + color.g * 0.534 + color.b * 0.131));

        pixels_data[i + 0] = new_r;
        pixels_data[i + 1] = new_g;
        pixels_data[i + 2] = new_b;
    }

    p.end = p.end_timer();

    static int counter = 0;

    counter++;
    p.report("report_sepia_tone_" + std::to_string(counter) + ".txt");
    exporter exporter;
    std::string filename = exporter.formater("export_sepia_tone_", &counter, ".png");

    if (stbi_write_png(filename.c_str(), loader.width, loader.height, loader.channels, pixels_data.data(), loader.width * loader.channels))
    {
        loader.texture_load(filename.c_str(), sdl_pstate->renderer, &sdl_pstate->src);
        return true;
    }
    return false;
}