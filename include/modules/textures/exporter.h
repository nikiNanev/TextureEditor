#pragma once

#include "common.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#endif

#include "textures/loader.h"

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

typedef struct _exporter
{
    bool is_exported{false};
    std::string output_directory{"../assets/images/"};
    std::string filename;
    double scale = 1.0f;

    // Image parameters
    int32_t width, height, channels, desired_channels = 4;
    std::vector<unsigned char> data;

    bool toPNG(const std::string filename);
    bool toJPEG(const std::string filename);
    bool toBMP(const std::string filename);

    std::string formater(const std::string filename, int *idx, const std::string format);
    bool load();
    void apply(const int format_idx, loader *loader, double scale = 1.0);

} exporter;
