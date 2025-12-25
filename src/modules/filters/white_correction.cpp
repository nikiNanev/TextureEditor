#include "filters/white_correction.h"
#include "logger/profiler.h"

bool white_correction::load(const std::string &filename, loader &loader)
{
    return loader.image_load(filename.c_str(), pixels_data);
}

bool white_correction::apply(loader &loader, sdl_state *sdl_pstate)
{
    // Profile the time consumption in the function
    profiler p;
    p.function = "White Correction";

    int pixel_index{0};

    int width = loader.width;
    int height = loader.height;
    int channels = loader.channels;

    long long sum_r = 0, sum_g = 0, sum_b = 0;

    p.start = p.start_timer();

    if (channels >= 3)
    {
        for (int i = 0; i < width; ++i)
        {
            for (int j = 0; j < height; ++j)
            {
                pixel_index = (i * height + j) * channels;

                sum_r += pixels_data[pixel_index + 0];
                sum_g += pixels_data[pixel_index + 1];
                sum_b += pixels_data[pixel_index + 2];

                double avg_r = (double)sum_r / (width * height);
                double avg_g = (double)sum_g / (width * height);
                double avg_b = (double)sum_b / (width * height);

                // Use the overall average as target (or fixed 128.0 for midpoint)
                double avg_gray = (avg_r + avg_g + avg_b) / 3.0;

                double gain_r = avg_gray / avg_r;
                double gain_g = avg_gray / avg_g;
                double gain_b = avg_gray / avg_b;

                // Apply gains and clamp to 0-255
                int r = (int)(pixels_data[pixel_index + 0] * gain_r);
                int g = (int)(pixels_data[pixel_index + 1] * gain_g);
                int b = (int)(pixels_data[pixel_index + 2] * gain_b);

                pixels_data[pixel_index + 0] = (r < 0) ? 0 : (r > 255) ? 255
                                                             : r;
                pixels_data[pixel_index + 1] = (g < 0) ? 0 : (g > 255) ? 255
                                                             : g;
                pixels_data[pixel_index + 2] = (b < 0) ? 0 : (b > 255) ? 255
                                                             : b;
            }
        }
    }

    p.end = p.end_timer();

    static int counter = 0;

    counter++;
    p.report("report_white_correction_" + std::to_string(counter) + ".txt");
    exporter exporter;
    std::string filename = exporter.formater("export_white_correction_", &counter, ".png");

    if (stbi_write_png(filename.c_str(), width, height, channels, pixels_data.data(), width * channels))
    {
        loader.texture_load(filename.c_str(), sdl_pstate->renderer, &sdl_pstate->src);
        return true;
    }
    return false;
}