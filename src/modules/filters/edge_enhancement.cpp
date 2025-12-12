#include "filters/edge_enhancement.h"

bool edge_enhancement::load(const std::string &filename, loader &loader)
{
    return loader.image_load(filename.c_str(), pixels_data);
}

bool edge_enhancement::apply(loader &loader, sdl_state *sdl_pstate)
{
    static int counter = 0;

    int idx{0};

    int width = loader.width;
    int height = loader.height;
    int channels = loader.channels;

    std::vector<unsigned char> output(width * height * channels);

    char kernel[3][3] =
        {
            {0, -1, 0},
            {-1, 5, -1},
            {0, -1, 0}};

    float sum{0.0f};

    for (int i = 1; i < width - 1; i++)
    {
        for (int j = 1; j < height - 1; j++)
        {

            sum = 0;

            for (int ki = -1; ki <= 1; ki++)
            {
                for (int kj = -1; kj <= 1; kj++)
                {
                    idx = ((i + ki) * width + (j + kj)) * channels;
                    float pixel_sum = pixels_data[idx + 0] + pixels_data[idx + 1] + pixels_data[idx + 2];
                    sum += kernel[ki + 1][kj + 1] * pixel_sum;
                }
            }

            // Clamp
            int new_value = (int)sum;
            new_value = (new_value < 0 ? 0 : (new_value > 255 ? 255 : new_value));
            output[i * width + j] = (unsigned char)new_value;
        }
    }

    counter++;
    exporter exporter;
    std::string filename = exporter.formater("export_edge_enhance_", &counter, ".png");

    stbi_write_png(filename.c_str(), width, height, channels, output.data(), width * channels);

    loader.texture_load(filename.c_str(), sdl_pstate->renderer, &sdl_pstate->src);

    // try
    // {

    //     dlib::array2d<dlib::rgb_alpha_pixel> output(image.nr() - 2, image.nc() - 2);

    //     char edge_enhance[3][3] =
    //         {
    //             {0, 0, 0},
    //             {-1, 1, 0},
    //             {0, 0, 0}};

    //     int sum_red = 0, sum_green = 0, sum_blue = 0;

    //     int i, j;

    //     for (i = 0; i < image.nr() - 2; i++)
    //     {
    //         for (j = 0; j < image.nc() - 2; j++)
    //         {

    //             sum_red = (image[i][j].red * edge_enhance[0][0] +
    //                        image[i][j + 1].red * edge_enhance[0][1] +
    //                        image[i][j + 2].red * edge_enhance[0][2] +
    //                        image[i + 1][j].red * edge_enhance[1][0] +
    //                        image[i + 1][j + 1].red * edge_enhance[1][1] +
    //                        image[i + 1][j + 2].red * edge_enhance[1][2] +
    //                        image[i + 2][j].red * edge_enhance[2][0] +
    //                        image[i + 2][j + 1].red * edge_enhance[2][1] +
    //                        image[i + 2][j + 2].red * edge_enhance[2][2]);

    //             sum_green = (image[i][j].green * edge_enhance[0][0] +
    //                          image[i][j + 1].green * edge_enhance[0][1] +
    //                          image[i][j + 2].green * edge_enhance[0][2] +
    //                          image[i + 1][j].green * edge_enhance[1][0] +
    //                          image[i + 1][j + 1].green * edge_enhance[1][1] +
    //                          image[i + 1][j + 2].green * edge_enhance[1][2] +
    //                          image[i + 2][j].green * edge_enhance[2][0] +
    //                          image[i + 2][j + 1].green * edge_enhance[2][1] +
    //                          image[i + 2][j + 2].green * edge_enhance[2][2]);

    //             sum_blue = (image[i][j].blue * edge_enhance[0][0] +
    //                         image[i][j + 1].blue * edge_enhance[0][1] +
    //                         image[i][j + 2].blue * edge_enhance[0][2] +
    //                         image[i + 1][j].blue * edge_enhance[1][0] +
    //                         image[i + 1][j + 1].blue * edge_enhance[1][1] +
    //                         image[i + 1][j + 2].blue * edge_enhance[1][2] +
    //                         image[i + 2][j].blue * edge_enhance[2][0] +
    //                         image[i + 2][j + 1].blue * edge_enhance[2][1] +
    //                         image[i + 2][j + 2].blue * edge_enhance[2][2]);

    //             output[i][j].red = sum_red / 9;
    //             output[i][j].green = sum_green / 9;
    //             output[i][j].blue = sum_blue / 9;
    //             output[i][j].alpha = image[i][j].alpha;
    //         }
    //     }

    //     idx++;

    //     exporter exporter;
    //     std::string filename = exporter.formater("export_edge_enchance_", &idx, ".png");

    //     save_png(output, filename);

    //     loader.texture_load(filename.c_str(), sdl_pstate->renderer, &sdl_pstate->src);

    //     return true;
    // }
    // catch (std::exception &e)
    // {
    //     std::cout << "exception thrown: " << e.what() << std::endl;
    //     return false;
    // }

    return true;
}