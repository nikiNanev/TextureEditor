#include "filters/tritone.h"

static inline unsigned char lerp_uc(unsigned char a, unsigned char b, float t)
{
    return (unsigned char)(a + (b - a) * t);
}

bool tritone::load(const std::string &filename, loader &loader)
{
    return loader.image_load(filename.c_str(), pixels_data);
}

bool tritone::apply(color &shadow, color &mid, color &highlight, loader &loader, sdl_state *sdl_pstate)
{
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

                float t = gray / 255.0f;

                unsigned char r, g, b;
                if (t < 0.5f)
                {
                    // Shadow to mid
                    float local_t = t * 2.0f;
                    r = lerp_uc(shadow.r, mid.r, local_t);
                    g = lerp_uc(shadow.g, mid.g, local_t);
                    b = lerp_uc(shadow.b, mid.b, local_t);
                }
                else
                {
                    // Mid to highlight
                    float local_t = (t - 0.5f) * 2.0f;
                    r = lerp_uc(mid.r, highlight.r, local_t);
                    g = lerp_uc(mid.g, highlight.g, local_t);
                    b = lerp_uc(mid.b, highlight.b, local_t);
                }

                pixels_data[pixel_index + 0] = r;
                pixels_data[pixel_index + 1] = g;
                pixels_data[pixel_index + 2] = b;
            }
        }
    }

    static int counter = 0;

    counter++;
    exporter exporter;
    std::string filename = exporter.formater("export_tritone_", &counter, ".png");

    if (stbi_write_png(filename.c_str(), width, height, channels, pixels_data.data(), width * channels))
    {
        loader.texture_load(filename.c_str(), sdl_pstate->renderer, &sdl_pstate->src);
        return true;
    }
    return false;
}