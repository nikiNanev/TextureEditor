#include <dlib/image_io.h>
#include <dlib/image_transforms.h>

#include "Modules/Textures/Exporter.h"
#include "Modules/Textures/Loader.h"
#include "Modules/States/EditorState.h"

class Blur
{
    dlib::array2d<dlib::rgb_pixel> image;
public:
    Blur()
    {
    }

    bool load(const std::string &filename);

    bool apply(const float &sigma, Loader &loader, sdl_state *sdl_pstate);
};