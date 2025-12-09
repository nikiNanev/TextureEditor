#include "filters/brightness_adjustment.h"

bool brightness_adjustment::load(const std::string &filename, loader &loader)
{
    return loader.image_load(filename.c_str(), pixel_data);
}

bool brightness_adjustment::apply(const float &alpha, const int &beta, loader &loader, sdl_state *sdl_pstate)
{

    static int counter = 0;

    // out_image.set_size(in_image.nr(), in_image.nc());

    // // %%% ouptut = alpha * pixel.channel + beta;

    // for (int i = 0; i < in_image.nr(); i++)
    // {
    //     for (int j = 0; j < in_image.nc(); j++)
    //     {

    //         out_image[i][j].red = alpha * in_image[i][j].red + beta;
    //         out_image[i][j].green = alpha * in_image[i][j].green + beta;
    //         out_image[i][j].blue = alpha * in_image[i][j].blue + beta;
    //         out_image[i][j].alpha = alpha * in_image[i][j].alpha + beta;
    //     }
    // }

    // counter++;
    // exporter exporter;
    // std::string filename = exporter.formater("export_brightness_adjustment_", &counter, ".png");

    // dlib::save_png(out_image, filename);

    // loader.texture_load(filename.c_str(), sdl_pstate->renderer, &sdl_pstate->src);

    return true;
}