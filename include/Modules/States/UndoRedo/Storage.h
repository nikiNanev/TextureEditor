#include "common.h"

#include <vector>

typedef struct textures
{
    SDL_Texture *texture;
    std::string filename;

} textures;

std::vector<textures> vtextures;
size_t current{0};