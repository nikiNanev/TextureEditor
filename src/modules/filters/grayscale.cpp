#include "filters/grayscale.h"
#include "logger/profiler.h"

bool grayscale::load(const std::string &filename, loader &loader)
{
    return loader.image_load(filename.c_str(), pixels_data);
}

bool grayscale::apply(loader &loader, sdl_state *sdl_pstate)
{

     // Profile the time consumption in the function
    profiler p;
    p.function = "Grayscale";

    int pixel_index{0};
    int output_index{0};

    int width = loader.width;
    int height = loader.height;
    int channels = loader.channels;

    std::vector<unsigned char> output_image(width * height);

    p.start = p.start_timer();

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            pixel_index = (i * width + j) * channels;
            output_index = (i * width + j);

            output_image[output_index] = static_cast<unsigned char>(0.2126 * pixels_data[pixel_index + 0] + 0.7152 * pixels_data[pixel_index + 1] + 0.0722 * pixels_data[pixel_index + 2] + 0.5f); // Rec. 709 ( HDTV ) Standard
        }
    }
    p.end = p.end_timer();

    static int counter = 0;

    counter++;
    p.report("report_grayscale_" + std::to_string(counter) + ".txt");
    
    exporter exporter;
    std::string filename = exporter.formater("export_grayscale_", &counter, ".png");

    if (stbi_write_png(filename.c_str(), width, height, 1, output_image.data(), width * 1))
    {
        loader.texture_load(filename.c_str(), sdl_pstate->renderer, &sdl_pstate->src);
        return true;
    }
    return false;
}