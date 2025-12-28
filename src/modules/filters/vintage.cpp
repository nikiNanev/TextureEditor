#include "filters/vintage.h"
#include "logger/profiler.h"

static unsigned char clamp(int v) {
    return (unsigned char)(v < 0 ? 0 : (v > 255 ? 255 : v));
}

bool vintage::load(const std::string &filename, loader &loader)
{
    return loader.image_load(filename.c_str(), pixels_data);
}

bool vintage::apply(loader &loader, sdl_state *sdl_pstate)
{
    // Profile the time consumption in the function
    profiler p;
    p.function = "Vintage";

    size_t size = loader.width * loader.height * loader.channels;

    // Apply vintage effects in-place
    for (int i = 0; i < size; i += 3)
    {
        color.r = pixels_data[i];
        color.g = pixels_data[i + 1];
        color.b = pixels_data[i + 2];

        // 1. Sepia tone (classic matrix)
        int new_r = clamp((int)(color.r * 0.393 + color.g * 0.769 + color.b * 0.189));
        int new_g = clamp((int)(color.r * 0.349 + color.g * 0.686 + color.b * 0.168));
        int new_b = clamp((int)(color.r * 0.272 + color.g * 0.534 + color.b * 0.131));

        // 2. Fade colors / reduce contrast (mix with gray)
        unsigned char gray = (unsigned char)(0.299 * new_r + 0.587 * new_g + 0.114 * new_b);
        new_r = (unsigned char)(new_r * 0.8 + gray * 0.2); // 80% sepia + 20% desaturated
        new_g = (unsigned char)(new_g * 0.8 + gray * 0.2);
        new_b = (unsigned char)(new_b * 0.8 + gray * 0.2);

        pixels_data[i] = new_r;
        pixels_data[i + 1] = new_g;
        pixels_data[i + 2] = new_b;
    }

    // 3. Vignette (darken edges)
    float center_x = loader.width / 2.0f;
    float center_y = loader.height / 2.0f;
    float max_dist = sqrtf(center_x * center_x + center_y * center_y);

    int pixel_index{0};

    p.start = p.start_timer();

    for (int i = 0; i < loader.width; ++i)
    {
        for (int j = 0; j < loader.height; ++j)
        {
            pixel_index = (i * loader.height + j) * loader.channels;

            float dx = i - center_x;
            float dy = j - center_y;
            float dist = sqrtf(dx * dx + dy * dy);

            float vignette = 1.0f - (dist / max_dist * 0.7f); // darken up to 70% at corners
            vignette = vignette < 0.3f ? 0.3f : vignette;     // keep center bright

            pixels_data[pixel_index] = clamp((int)(pixels_data[pixel_index] * vignette));
            pixels_data[pixel_index + 1] = clamp((int)(pixels_data[pixel_index + 1] * vignette));
            pixels_data[pixel_index + 2] = clamp((int)(pixels_data[pixel_index + 2] * vignette));
        }
    }

    p.end = p.end_timer();

    static int counter = 0;

    counter++;
    p.report("report_vintage_" + std::to_string(counter) + ".txt");
    exporter exporter;
    std::string filename = exporter.formater("export_vintage_", &counter, ".png");

    if (stbi_write_png(filename.c_str(), loader.width, loader.height, loader.channels, pixels_data.data(), loader.width * loader.channels))
    {
        loader.texture_load(filename.c_str(), sdl_pstate->renderer, &sdl_pstate->src);
        return true;
    }
    return false;
}