#include "filters/duotone.h"

static inline unsigned char lerp_uc(unsigned char a, unsigned char b, float t)
{
    return (unsigned char)(a + (b - a) * t);
}

bool duotone::load(const std::string &filename, loader &loader)
{
    return loader.image_load(filename.c_str(), pixels_data);
}

bool duotone::apply(color &shadow, color &highlight, loader &loader, sdl_state *sdl_pstate)
{
    // Profiling the time consumption of the function
    profiler p;
    p.function = "Duotone";
    p.start = p.start_timer();

    int pixel_index{0};

    int width = loader.width;
    int height = loader.height;
    int channels = loader.channels;

    if (channels >= 3)
    {
        for (int i = 0; i < height; ++i)
        {
            for (int j = 0; j < width; ++j)
            {
                pixel_index = (i * width + j) * channels;

                unsigned char gray = (unsigned char)(0.2126f * pixels_data[pixel_index + 0] +
                                                     0.7152f * pixels_data[pixel_index + 1] +
                                                     0.5870f * pixels_data[pixel_index + 2]);

                float t = gray / 255.0f; // 0.0 (dark) to 1.0 (light)

                unsigned char r = lerp_uc(shadow.r, highlight.r, t);
                unsigned char g = lerp_uc(shadow.g, highlight.g, t);
                unsigned char b = lerp_uc(shadow.b, highlight.b, t);

                pixels_data[pixel_index + 0] = r;
                pixels_data[pixel_index + 1] = g;
                pixels_data[pixel_index + 2] = b;
            }
        }
    }

    p.end = p.end_timer();
    // End of the profiling

    static int counter = 0;

    counter++;

    p.report("report_duotone_" + std::to_string(counter) + ".txt");
    std::cout << "Done Writing!" << std::endl;

    exporter exporter;
    std::string filename = exporter.formater("export_duotone_", &counter, ".png");

    if (stbi_write_png(filename.c_str(), width, height, channels, pixels_data.data(), width * channels))
    {
        loader.texture_load(filename.c_str(), sdl_pstate->renderer, &sdl_pstate->src);
        return true;
    }
    return false;
}