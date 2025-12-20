#include "filters/blur.h"

#ifndef IIR_GAUSS_BLUR_IMPLEMENTATION
#define IIR_GAUSS_BLUR_IMPLEMENTATION
#include "iir_gauss_blur.h"

#endif

bool blur::load(const std::string &filename, loader &loader)
{
    return loader.image_load(filename.c_str(), pixels_data);
}

bool blur::apply(const float &sigma, loader &loader, sdl_state *sdl_pstate)
{
    int pixel_index{0};

    iir_gauss_blur(loader.width, loader.height, loader.channels, pixels_data.data(), sigma);

    static int counter = 0;

    counter++;
    exporter exporter;
    std::string filename = exporter.formater("export_gaussian_blur_", &counter, ".png");

    if (stbi_write_png(filename.c_str(), loader.width, loader.height, loader.channels, pixels_data.data(), loader.width * loader.channels))
    {
        loader.texture_load(filename.c_str(), sdl_pstate->renderer, &sdl_pstate->src);
        return true;
    }
    return false;
}
