#pragma once

#include "common.h"

#include "Modules/Textures/Loader.h"

#include <dlib/image_io.h>

static int idx{0};

typedef enum
{
    PNG_FORMAT,
    JPEG_FORMAT,
    BMP_FORMAT,
    WEBP_FORMAT,
    DNG_FORMAT,
} formats;

class Exporter
{

private:
    bool is_exported{false};
    std::string filename;
public:
    Exporter(){}

    bool toPNG(const std::string filename);
    bool toJPEG();
    bool toBMP();

    void dlib_exporter(const int format_idx, Loader *loader);
};