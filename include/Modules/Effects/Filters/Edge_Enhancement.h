#pragma once

#include <dlib/image_io.h>

#include "Modules/Textures/Exporter.h"
#include "Modules/Textures/Loader.h"
#include "Modules/States/EditorState.h"

class Edge_Enhancement
{
    dlib::array2d<dlib::rgb_alpha_pixel> image;

public:
    Edge_Enhancement()
    {

    }

    bool load(const std::string &filename);
    bool apply(Loader &loader, sdl_state *sdl_pstate);

};