#include "Modules/Textures/Exporter.h"

bool Exporter::toPNG(const std::string filename)
{
    try
    {
        dlib::array2d<dlib::rgb_pixel> image;
        dlib::load_image(image, filename.c_str());

        idx_png++;

        this->filename = this->formater("export_png_", &idx_png, ".png");

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

bool Exporter::toJPEG(const std::string filename)
{
    try
    {
        dlib::array2d<dlib::rgb_pixel> image;
        dlib::load_image(image, filename.c_str());

        idx_jpeg++;

        this->filename = formater("export_jpeg_", &idx_jpeg, ".jpeg");

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

bool Exporter::toBMP(const std::string filename)
{
    try
    {
        dlib::array2d<dlib::rgb_pixel> image;
        dlib::load_image(image, this->filename.c_str());

        idx_bmp++;

        this->filename = formater("export_bmp_", &idx_bmp, ".bmp");

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
            dlib::load_image(image, loader->get_filename_path());

            idx_png++;

            this->filename = formater("export_png_", &idx_png, ".png");

            dlib::save_png(image, this->filename);

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
            dlib::load_image(image, loader->get_filename_path());

            idx_jpeg++;

            this->filename = formater("export_jpeg_", &idx_jpeg, ".jpeg");
            
            dlib::save_jpeg(image, this->filename);
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
            dlib::load_image(image, loader->get_filename_path());

            idx_bmp++;

            this->filename = formater("export_bmp_", &idx_bmp, ".bmp");

            dlib::save_bmp(image, this->filename);

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
            dlib::load_image(image, loader->get_filename_path());

            idx_webp++;

            this->filename = formater("export_webp_", &idx_webp, ".webp");

            dlib::save_webp(image, this->filename);

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
            dlib::load_image(image, loader->get_filename_path());

            idx_dng++;

            this->filename = formater("export_dng_", &idx_dng, ".dng");

            dlib::save_dng(image, this->filename);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    break;
    }
}

std::string Exporter::formater(const std::string filename, int *idx, const std::string format)
{

    std::string output = this->output_directory;
    output.append(filename);
    output.append(std::to_string(*idx));
    output.append(format);

    return output;
}