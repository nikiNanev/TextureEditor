#include "filters/edge_enhancement.h"
#include "logger/profiler.h"

bool edge_enhancement::load(const std::string &filename, loader &loader)
{
    return loader.image_load(filename.c_str(), pixels_data);
}

bool edge_enhancement::apply(loader &loader, sdl_state *sdl_pstate)
{
    // Profile the time consumption in the function
    profiler p;
    p.function = "Edge Enhancement";

    static int counter = 0;

    int idx{0};

    int width = loader.width;
    int height = loader.height;
    int channels = loader.channels;

    std::vector<unsigned char> output(width * height * channels);

    char kernel[3][3] =
        {
            {0, -1, 0},
            {-1, 5, -1},
            {0, -1, 0}};

    float sum{0.0f};

    p.start = p.start_timer();
    for (int i = 1; i < width - 1; i++)
    {
        for (int j = 1; j < height - 1; j++)
        {

            sum = 0;

            for (int ki = -1; ki <= 1; ki++)
            {
                for (int kj = -1; kj <= 1; kj++)
                {
                    idx = ((i + ki) * width + (j + kj)) * channels;
                    float pixel_sum = pixels_data[idx + 0] + pixels_data[idx + 1] + pixels_data[idx + 2];
                    sum += kernel[ki + 1][kj + 1] * pixel_sum;
                }
            }

            // Clamp
            int new_value = (int)sum;
            new_value = (new_value < 0 ? 0 : (new_value > 255 ? 255 : new_value));
            output[i * width + j] = (unsigned char)new_value;
        }
    }
    p.end = p.end_timer();

    counter++;
    p.report("report_edge_enhancement_" + std::to_string(counter) + ".txt");

    exporter exporter;
    std::string filename = exporter.formater("export_edge_enhance_", &counter, ".png");

    stbi_write_png(filename.c_str(), width, height, channels, output.data(), width * channels);

    loader.texture_load(filename.c_str(), sdl_pstate->renderer, &sdl_pstate->src);

    return true;
}