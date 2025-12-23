#include "filters/gamma_correction.h"
#include "logger/profiler.h"

bool gamma_correction::load(const std::string &filename, loader &loader)
{
    return loader.image_load(filename.c_str(), pixels_data);
}

bool gamma_correction::apply(loader &loader, sdl_state *sdl_pstate, float &gamma)
{
    // Profile the time consumption in the function
    profiler p;
    p.function = "Gamma Correction";

    int pixel_index{0};

    int width = loader.width;
    int height = loader.height;
    int channels = loader.channels;

    float gamma_corr = 1.f / gamma;

    p.start = p.start_timer();

    if (channels >= 3)
    {
        for (int i = 0; i < height; ++i)
        {
            for (int j = 0; j < width; ++j)
            {
                pixel_index = (i * width + j) * channels;

                pixels_data[pixel_index + 0] = 256.f * std::pow(pixels_data[pixel_index + 0] / 256.f, gamma_corr);
                pixels_data[pixel_index + 1] = 256.f * std::pow(pixels_data[pixel_index + 1] / 256.f, gamma_corr);
                pixels_data[pixel_index + 2] = 256.f * std::pow(pixels_data[pixel_index + 2] / 256.f, gamma_corr);
            }
        }
    }
    p.end = p.end_timer();

    static int counter = 0;

    counter++;
    p.report("report_gamma_correction_" + std::to_string(counter) + ".txt");
    
    exporter exporter;
    std::string filename = exporter.formater("export_gamma_correction_", &counter, ".png");

    if (stbi_write_png(filename.c_str(), width, height, channels, pixels_data.data(), width * channels))
    {
        loader.texture_load(filename.c_str(), sdl_pstate->renderer, &sdl_pstate->src);
        return true;
    }
    return false;
}