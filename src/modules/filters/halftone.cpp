#include "filters/halftone.h"

#define PI 3.14159265359f
#define FREQUENCY 25.0f // Higher = smaller/finer dots (15-40 recommended)

int cell_size;

// Draw colored dot (for CMYK)
void halftone::draw_dot(unsigned char *out, int cx, int cy, int width, int height, int channels,
                        float radius, unsigned char r, unsigned char g, unsigned char b)
{
    int r2 = (int)(radius * radius + 0.5f);
    for (int dy = -(int)radius; dy <= (int)radius; dy++)
    {
        for (int dx = -(int)radius; dx <= (int)radius; dx++)
        {
            if (dx * dx + dy * dy <= r2)
            {
                int px = cx + dx;
                int py = cy + dy;
                if (px >= 0 && px < width && py >= 0 && py < height)
                {
                    int idx = (py * width + px) * channels;
                    // Additive blend (screen-like) - brighter where dots overlap
                    out[idx + 0] = 255 - (255 - out[idx + 0]) * (255 - r) / 255;
                    out[idx + 1] = 255 - (255 - out[idx + 1]) * (255 - g) / 255;
                    out[idx + 2] = 255 - (255 - out[idx + 2]) * (255 - b) / 255;
                    if(channels == 4)
                        out[idx + 3] = 255;
                }
            }
        }
    }
}

// Halftone one channel with rotation angle
void halftone::halftone_channel(const unsigned char *intensity_map, unsigned char *out,
                                int width, int height, int channels, float angle_deg,
                                unsigned char dot_r, unsigned char dot_g, unsigned char dot_b)
{
    float angle = angle_deg * PI / 180.0f;
    float cos_a = cosf(angle);
    float sin_a = sinf(angle);

    for (int base_y = 0; base_y < height; base_y += cell_size)
    {
        for (int base_x = 0; base_x < width; base_x += cell_size)
        {
            // Rotated grid sampling
            float offset_x = cos_a * (base_x - width / 2.0f) + sin_a * (base_y - height / 2.0f);
            float offset_y = -sin_a * (base_x - width / 2.0f) + cos_a * (base_y - height / 2.0f);
            int sample_x = base_x + cell_size / 2 + (int)(offset_x * 0.15f);
            int sample_y = base_y + cell_size / 2 + (int)(offset_y * 0.15f);
            sample_x = sample_x < 0 ? 0 : (sample_x >= width ? width - 1 : sample_x);
            sample_y = sample_y < 0 ? 0 : (sample_y >= height ? height - 1 : sample_y);

            unsigned char intensity = intensity_map[sample_y * width + sample_x];
            float t = intensity / 255.0f;
            float max_r = cell_size * 0.45f;
            float radius = max_r * sqrtf(t);

            if (radius > 1.0f)
            {
                int center_x = base_x + cell_size / 2;
                int center_y = base_y + cell_size / 2;
                draw_dot(out, center_x, center_y, width, height, channels, radius, dot_r, dot_g, dot_b);
            }
        }
    }
}

bool halftone::load(const std::string &filename, loader &loader)
{
    return loader.image_load(filename.c_str(), pixels_data);
}

bool halftone::apply(loader &loader, sdl_state *sdl_pstate)
{

    // Profile the time consumption in the function
    profiler p;
    p.function = "Halftone";

    cell_size = (int)(150.0f / FREQUENCY);

    std::vector<unsigned char> c_map(loader.width * loader.height);
    std::vector<unsigned char> m_map(loader.width * loader.height);
    std::vector<unsigned char> y_map(loader.width * loader.height);
    std::vector<unsigned char> k_map(loader.width * loader.height);

    for (int i = 0; i < loader.width * loader.height; i++)
    {
        unsigned char r = pixels_data[i * loader.channels + 0];
        unsigned char g = pixels_data[i * loader.channels + 1];
        unsigned char b = pixels_data[i * loader.channels + 2];
        unsigned char k = (r < g ? (r < b ? r : b) : (g < b ? g : b)); // Min for black
        c_map[i] = 255 - r;
        m_map[i] = 255 - g;
        y_map[i] = 255 - b;
        k_map[i] = k;
    }

    std::vector<unsigned char> output(loader.width * loader.height * loader.channels);
    std::vector<unsigned char> temp(loader.width * loader.height * loader.channels);

    // Cyan (15°)
    halftone_channel(c_map.data(), temp.data(), loader.width, loader.height, loader.channels, 15.0f, 0, 255, 255);
    for (int i = 0; i < loader.width * loader.height * loader.channels; i++)
        output[i] = temp[i] > output[i] ? temp[i] : output[i];

    // Magenta (75°)
    halftone_channel(m_map.data(), temp.data(), loader.width, loader.height, loader.channels, 75.0f, 255, 0, 255);
    for (int i = 0; i < loader.width * loader.height * loader.channels; i++)
        output[i] = temp[i] > output[i] ? temp[i] : output[i];

    // Yellow (0°)
    halftone_channel(y_map.data(), temp.data(), loader.width, loader.height, loader.channels, 0.0f, 255, 255, 0);
    for (int i = 0; i < loader.width * loader.height * loader.channels; i++)
        output[i] = temp[i] > output[i] ? temp[i] : output[i];

    // Black (35°)
    halftone_channel(k_map.data(), temp.data(), loader.width, loader.height, loader.channels, 45.0f, 0, 0, 0);
    for (int i = 0; i < loader.width * loader.height * loader.channels; i++)
        output[i] = temp[i] < output[i] ? temp[i] : output[i]; // Darker wins for black

    static int counter = 0;

    counter++;
    p.report("report_halftone_" + std::to_string(counter) + ".txt");

    exporter exporter;
    std::string filename = exporter.formater("export_halftone_", &counter, ".png");

    if (stbi_write_png(filename.c_str(), loader.width, loader.height, loader.channels, output.data(), loader.width * loader.channels))
    {
        loader.texture_load(filename.c_str(), sdl_pstate->renderer, &sdl_pstate->src);
        return true;
    }

    return false;
}