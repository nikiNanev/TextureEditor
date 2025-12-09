#include "filters/blur.h"

bool blur::load(const std::string &filename, loader &loader)
{
    return loader.image_load(filename.c_str(), pixels_data);
}

bool blur::apply(const float &sigma, loader &loader, sdl_state *sdl_pstate)
{
    static int counter = 0;
    try
    {

        //Gaussian Blur with stb

        counter++;

        exporter exporter;
        std::string filename = exporter.formater("export_blur_", &counter, ".png");
        
        exporter.toPNG(filename);

        loader.texture_load(filename.c_str(), sdl_pstate->renderer, &sdl_pstate->src);

        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
}
