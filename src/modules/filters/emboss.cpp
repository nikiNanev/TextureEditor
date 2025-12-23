#include "filters/emboss.h"
#include "logger/profiler.h"

bool emboss::load(const std::string &filename, loader &loader)
{
    return loader.image_load(filename.c_str(), pixels_data);
}

bool emboss::apply(loader &loader, sdl_state *sdl_pstate)
{
    // Profile the time consumption in the function
    profiler p;
    p.function = "Emboss";

    static int counter = 0;

    int idx{0};

    int width = loader.width;
    int height = loader.height;
    int channels = loader.channels;

    std::vector<unsigned char> output(width * height * 3);

    char kernel[3][3] =
        {
            {1, 0, 0},
            {0, 0, 0},
            {0, 0, -1}};

    p.start = p.start_timer();

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            float r = 0.0f, g = 0.0f, b = 0.0f;

            // Apply 3x3 kernel
            for (int ky = -1; ky <= 1; ky++)
            {
                for (int kx = -1; kx <= 1; kx++)
                {
                    int iy = y + ky;
                    int ix = x + kx;

                    // Clamp coordinates (simple replicate border)
                    if (iy < 0)
                        iy = 0;
                    if (iy >= height)
                        iy = height - 1;
                    if (ix < 0)
                        ix = 0;
                    if (ix >= width)
                        ix = width - 1;

                    int idx = (iy * width + ix) * channels;
                    float weight = kernel[ky + 1][kx + 1];

                    r += pixels_data[idx + 0] * weight;
                    g += pixels_data[idx + 1] * weight;
                    b += pixels_data[idx + 2] * weight;
                }
            }

            // Clamp to 0..255
            int out_r = (int)roundf(fmaxf(0, fminf(255, r)));
            int out_g = (int)roundf(fmaxf(0, fminf(255, g)));
            int out_b = (int)roundf(fmaxf(0, fminf(255, b)));

            int out_idx = (y * width + x) * 3;
            output[out_idx + 0] = (unsigned char)out_r;
            output[out_idx + 1] = (unsigned char)out_g;
            output[out_idx + 2] = (unsigned char)out_b;
        }
    }
    p.end = p.end_timer();

    counter++;
    p.report("report_emboss_" + std::to_string(counter) + ".txt");
    
    exporter exporter;
    std::string filename = exporter.formater("export_emboss_", &counter, ".png");

    stbi_write_png(filename.c_str(), width, height, 3, output.data(), width * 3);

    loader.texture_load(filename.c_str(), sdl_pstate->renderer, &sdl_pstate->src);

    return true;
}