#include "Modules/Textures/Exporter.h"

bool Exporter::toPNG(const std::string filename)
{
    try
    {
        dlib::array2d<dlib::rgb_pixel> image;
        dlib::load_image(image, filename.c_str());

        std::string output = "default_export_";
        output.append(std::to_string(idx++));
        output.append(".png");
        this->filename = output;

        dlib::save_png(image, this->filename);

        this->is_exported = true;
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    this->is_exported = false;
    return false;
}

bool Exporter::toJPEG()
{
    try
    {
        std::string output = "default_export_";
        output.append(std::to_string(idx++));
        output.append(".jpg");
        this->filename = output;
        dlib::array2d<dlib::rgb_pixel> image;
        dlib::load_image(image, this->filename.c_str());

        dlib::save_jpeg(image, this->filename);

        this->is_exported = true;
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    this->is_exported = false;
    return false;
}

bool Exporter::toBMP()
{
    try
    {
        std::string output = "default_export_";
        output.append(std::to_string(idx++));
        output.append(".bmp");
        this->filename = output;
        dlib::array2d<dlib::rgb_pixel> image;
        dlib::load_image(image, this->filename.c_str());

        dlib::save_bmp(image, this->filename);

        this->is_exported = true;
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    this->is_exported = false;
    return false;
}

void Exporter::dlib_exporter(const int format_idx, Loader *loader)
{
    switch (format_idx)
    {
    case PNG_FORMAT:
    {
        try
        {
            dlib::array2d<dlib::rgb_pixel> image;
            dlib::load_image(image, loader->get_file_path());

            const std::string file_output = "exported_dlib.png";
            dlib::save_png(image, file_output);

            std::cout << "Success export with dlib to png format" << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    break;
    case JPEG_FORMAT:
    {
        try
        {
            dlib::array2d<dlib::rgb_pixel> image;
            dlib::load_image(image, loader->get_file_path());

            const std::string file_output = "exported_dlib.jpeg";
            dlib::save_jpeg(image, file_output);

            std::cout << "Success export with dlib to jpeg format" << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    break;
    case BMP_FORMAT:
    {
        try
        {
            dlib::array2d<dlib::rgb_pixel> image;
            dlib::load_image(image, loader->get_file_path());

            const std::string file_output = "exported_dlib.bmp";
            dlib::save_bmp(image, file_output);

            std::cout << "Success export with dlib to bmp format" << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    break;
    case WEBP_FORMAT:
    {
        try
        {
            dlib::array2d<dlib::rgb_pixel> image;
            dlib::load_image(image, loader->get_file_path());

            const std::string file_output = "exported_dlib.webp";
            dlib::save_webp(image, file_output);

            std::cout << "Success export with dlib to webp format" << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    break;
    case DNG_FORMAT:
    {
        try
        {
            dlib::array2d<dlib::rgb_pixel> image;
            dlib::load_image(image, loader->get_file_path());

            const std::string file_output = "exported_dlib.dng";
            dlib::save_dng(image, file_output);

            std::cout << "Success export with dlib to dng format" << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    break;
    }
}