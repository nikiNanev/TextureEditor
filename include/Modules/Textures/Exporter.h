#pragma once

#include "common.h"

#include "Modules/Textures/Loader.h"

#include <dlib/image_io.h>
#include <dlib/image_transforms.h>

static int idx_png{0};
static int idx_jpeg{0};
static int idx_bmp{0};
static int idx_dng{0};
static int idx_webp{0};

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
    std::string output_directory{"../assets/"};
    std::string filename;
    double scale = 1.0f;
public:
    Exporter(){}

    bool toPNG(const std::string filename);
    bool toJPEG(const std::string filename);
    bool toBMP(const std::string filename);

    std::string formater(const std::string filename, int *idx, const std::string format);

    void dlib_exporter(const int format_idx, Loader *loader, double scale = 1.0);
};