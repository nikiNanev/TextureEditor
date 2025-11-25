#include "Modules/Effects/Filters/Blur.h"

bool Blur::load(const std::string &filename)
{
    try
    {
        dlib::load_image(image, filename);
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
}

bool Blur::apply(const float &sigma, Loader &loader, sdl_state *sdl_pstate)
{
    static int counter = 0;
    try
    {

        dlib::gaussian_blur(
            image,
            image,
            sigma,
            500);

        counter++;

        Exporter exporter;
        std::string filename = exporter.formater("export_blur_", &counter, ".png");
        
        dlib::save_png(image, filename);

        loader.texture_load(filename.c_str(), sdl_pstate->renderer, &sdl_pstate->src);

        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
}
