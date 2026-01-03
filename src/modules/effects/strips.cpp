#include "effects/strips.h"
#include "logger/profiler.h"

bool strips::load(const std::string &filename, loader &loader)
{
    return loader.image_load(filename.c_str(), image);
}

bool strips::apply(loader &loader, sdl_state &sdl_pstate)
{
    // Profile the time consumption in the function
    profiler p;
    p.function = "Strips";

    p.start = p.start_timer();

    if (horizontal_strips > 0 || vertical_strips > 0)
    {
        idx = 0;

        image.reserve(loader.width * loader.height * loader.channels);

        int h_parts[horizontal_strips];
        int v_parts[vertical_strips];

        int step = std::ceil(loader.height / horizontal_strips);

        int step_curr = step;
        std::cout << "step h: ";
        for (int i = 0; i < horizontal_strips; i++)
        {
            h_parts[i] = step_curr;
            step_curr += step;

            std::cout << step_curr << " ";
        }

        std::cout << std::endl;

        step = std::ceil(loader.width / vertical_strips);
        step_curr = step;

        std::cout << "step v: ";
        for (int i = 0; i < vertical_strips; i++)
        {
            v_parts[i] = step_curr;
            step_curr += step;

            std::cout << step_curr << " ";
        }

        std::cout << std::endl;

        bool is_strip_located = false;

        for (int i = 0; i < loader.width; i++)
        {
            for (int j = 0; j < loader.height; j++)
            {
                idx = (i * loader.height + j) * loader.channels;

                for (int k = 0; k < horizontal_strips; k++)
                {
                    if (j == h_parts[k])
                    {
                        is_strip_located = true;
                        break;
                    }
                }

                for (int k = 0; k < vertical_strips; k++)
                {
                    if (i == v_parts[k])
                    {
                        is_strip_located = true;
                        break;
                    }
                }

                if (is_strip_located)
                {
                    image[idx + 0] = color.r;
                    image[idx + 1] = color.g;
                    image[idx + 2] = color.b;

                    if (loader.channels == 4)
                        image[idx + 3] = 255;

                    is_strip_located = false;
                }
                else
                {
                    image[idx + 0] = loader.pixels_data[idx + 0];
                    image[idx + 1] = loader.pixels_data[idx + 1];
                    image[idx + 2] = loader.pixels_data[idx + 2];

                    if (loader.channels == 4)
                        image[idx + 3] = loader.pixels_data[idx + 3];
                }
            }
        }
    }

    p.end = p.end_timer();

    static int counter = 0;

    counter++;

    p.report("report_splitter_" + std::to_string(counter) + ".txt");

    exporter exporter;
    std::string filename = exporter.formater("export_splitter_", &counter, ".png");

    if (stbi_write_png(filename.c_str(), loader.width, loader.height, loader.channels, image.data(), loader.width * loader.channels))
    {
        loader.texture_load(filename.c_str(), sdl_pstate.renderer, &sdl_pstate.src);
        return true;
    }
    return false;
}