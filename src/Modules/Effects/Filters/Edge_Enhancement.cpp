#include "Modules/Effects/Filters/Edge_Enhancement.h"

bool Edge_Enhancement::load(const std::string &filename)
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

bool Edge_Enhancement::apply(int &idx)
{
    try
    {

        dlib::array2d<dlib::rgb_alpha_pixel> output(image.nr() - 2, image.nc() - 2);
        
        char edge_enhance[3][3] =
            {
                {0, 0, 0},
                {-1, 1, 0},
                {0, 0, 0}};

        int sum_red = 0, sum_green = 0, sum_blue = 0;

        int i, j;

        for (i = 0; i < image.nr() - 2; i++)
        {
            for (j = 0; j < image.nc() - 2; j++)
            {

                sum_red = (image[i][j].red * edge_enhance[0][0] +
                           image[i][j + 1].red * edge_enhance[0][1] +
                           image[i][j + 2].red * edge_enhance[0][2] +
                           image[i + 1][j].red * edge_enhance[1][0] +
                           image[i + 1][j + 1].red * edge_enhance[1][1] +
                           image[i + 1][j + 2].red * edge_enhance[1][2] +
                           image[i + 2][j].red * edge_enhance[2][0] +
                           image[i + 2][j + 1].red * edge_enhance[2][1] +
                           image[i + 2][j + 2].red * edge_enhance[2][2]);

                sum_green = (image[i][j].green * edge_enhance[0][0] +
                             image[i][j + 1].green * edge_enhance[0][1] +
                             image[i][j + 2].green * edge_enhance[0][2] +
                             image[i + 1][j].green * edge_enhance[1][0] +
                             image[i + 1][j + 1].green * edge_enhance[1][1] +
                             image[i + 1][j + 2].green * edge_enhance[1][2] +
                             image[i + 2][j].green * edge_enhance[2][0] +
                             image[i + 2][j + 1].green * edge_enhance[2][1] +
                             image[i + 2][j + 2].green * edge_enhance[2][2]);

                sum_blue = (image[i][j].blue * edge_enhance[0][0] +
                            image[i][j + 1].blue * edge_enhance[0][1] +
                            image[i][j + 2].blue * edge_enhance[0][2] +
                            image[i + 1][j].blue * edge_enhance[1][0] +
                            image[i + 1][j + 1].blue * edge_enhance[1][1] +
                            image[i + 1][j + 2].blue * edge_enhance[1][2] +
                            image[i + 2][j].blue * edge_enhance[2][0] +
                            image[i + 2][j + 1].blue * edge_enhance[2][1] +
                            image[i + 2][j + 2].blue * edge_enhance[2][2]);

                output[i][j].red = sum_red / 9;
                output[i][j].green = sum_green / 9;
                output[i][j].blue = sum_blue / 9;
                output[i][j].alpha = image[i][j].alpha;
            }
        }

        std::string filename_output = "../assets/export_edge_enhance";
        filename_output.append(std::to_string(idx++));
        filename_output.append(".png");

        save_png(output, filename_output);
        return true;
    }
    catch (std::exception &e)
    {
        std::cout << "exception thrown: " << e.what() << std::endl;
        return false;
    }
}