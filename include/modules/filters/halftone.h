#include "textures/exporter.h"
#include "textures/loader.h"
#include "states/editor_state.h"
#include "logger/profiler.h"
#include "utilities/color.h"

typedef struct _halftone
{
    std::vector<unsigned char> pixels_data;

    bool load(const std::string &filename, loader &loader);
    bool apply(loader &loader, sdl_state *sdl_pstate);

    void draw_dot(unsigned char *out, int cx, int cy, int width, int height, int channels,
                  float radius, unsigned char r, unsigned char g, unsigned char b);
    void halftone_channel(const unsigned char *intensity_map, unsigned char *out,
                     int width, int height, int channels, float angle_deg,
                     unsigned char dot_r, unsigned char dot_g, unsigned char dot_b);
} halftone;