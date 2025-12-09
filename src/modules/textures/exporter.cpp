#include "modules/textures/exporter.h"

bool exporter::load()
{
    uint8_t desired_channels = 4;

    unsigned char *loaded_data = stbi_load(
        filename.c_str(),
        &width,
        &height,
        &channels,
        desired_channels);

    if (loaded_data)
    {
        // desired_channels is 0 if we kept original, otherwise it's the forced value
        channels = desired_channels != 0 ? desired_channels : channels;

        size_t size = static_cast<size_t>(width) * height * channels;
        data.assign(loaded_data, loaded_data + size);

        // Free the memory allocated by stb_image
        stbi_image_free(loaded_data);

        this->is_exported = true;
        return true;
    }
    else
    {
        std::cout << "Failed to load image: " + filename + " - " + stbi_failure_reason() << std::endl;

        this->is_exported = false;
        return false;
    }
}

bool exporter::toPNG(const std::string filename)
{
    if (this->load())
    {
        this->filename = formater("export_png_", &idx_png, ".png");
        stbi_write_png(this->filename.c_str(), width, height, 1, data.data(), width);
        return true;
    }
    return false;
}

bool exporter::toJPEG(const std::string filename)
{
    if (this->load())
    {
        this->filename = formater("export_jpeg_", &idx_png, ".jpeg");
        stbi_write_jpg(this->filename.c_str(), width, height, 1, data.data(), 80);
        return true;
    }
    return false;
}

bool exporter::toBMP(const std::string filename)
{
    if (this->load())
    {
        this->filename = formater("export_bmp_", &idx_png, ".bmp");
        stbi_write_bmp(this->filename.c_str(), width, height, 1, data.data());
        return true;
    }

    return false;
}

void exporter::apply(const int format_idx, loader *loader, double scale)
{
    switch (format_idx)
    {
    case PNG_FORMAT:
    {
        this->toPNG(loader->filename_path);
    }
    break;
    case JPEG_FORMAT:
    {
        this->toJPEG(loader->filename_path);
    }
    break;
    case BMP_FORMAT:
    {
        this->toBMP(loader->filename_path);
    }
    break;
    }
}

std::string exporter::formater(const std::string filename, int *idx, const std::string format)
{

    std::string output = this->output_directory;
    output.append(filename);
    output.append(std::to_string(*idx));
    output.append(format);

    return output;
}