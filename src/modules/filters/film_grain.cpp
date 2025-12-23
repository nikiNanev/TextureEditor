#include "filters/film_grain.h"
#include "logger/profiler.h"

bool film_grain::load(const std::string &filename, loader &loader)
{
    return loader.image_load(filename.c_str(), pixels_data);
}

bool film_grain::apply(loader &loader, sdl_state *sdl_pstate, int &strength)
{
    // Profile the time consumption in the function
    profiler p;
    p.function = "Film Grain";

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

                for (int c = 0; c < channels; ++c)
                {
                    int noise = (std::rand() % (strength * 2 + 1)) - strength; // -strength..+strength
                    int val = pixels_data[pixel_index + c] + noise;

                    pixels_data[pixel_index + c] = static_cast<unsigned char>(
                        val < 0 ? 0 : (val > 255 ? 255 : val));
                }
            }
        }
    }
    p.end = p.end_timer();

    static int counter = 0;

    counter++;
    p.report("report_film_grain_" + std::to_string(counter) + ".txt");
    exporter exporter;
    std::string filename = exporter.formater("export_film_grain_", &counter, ".png");

    if (stbi_write_png(filename.c_str(), width, height, channels, pixels_data.data(), width * channels))
    {
        loader.texture_load(filename.c_str(), sdl_pstate->renderer, &sdl_pstate->src);
        return true;
    }
    return false;
}