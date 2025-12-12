#include <string>
#include <iostream>
#include <vector>

typedef struct _binary_thresholds
{

    int width = 0;
    int height = 0;
    int channels = 0; // original channels in file (1–4)
    int actual_channels = 0;
    std::vector<unsigned char> data;

    // Helper to get pixel (x,y) as unsigned char pointer (top-left origin)
    const unsigned char *getPixel(int x, int y) const
    {
        if (x < 0 || x >= width || y < 0 || y >= height)
            return nullptr;
        return &data[(y * width + x) * channels];
    }

    bool load(const std::string &filename, int desired_channels = 4)
    {
        unsigned char *loaded_data = stbi_load(
            filename.c_str(),
            &width,
            &height,
            &channels,
            desired_channels);

        if (!loaded_data)
        {
            std::cout << "Failed to load image: " + filename + " - " + stbi_failure_reason() << std::endl;
            return false;
        }

        // desired_channels is 0 if we kept original, otherwise it's the forced value
        actual_channels = desired_channels != 0 ? desired_channels : channels;

        size_t size = static_cast<size_t>(width) * height * actual_channels;
        data.assign(loaded_data, loaded_data + size);

        // Free the memory allocated by stb_image
        stbi_image_free(loaded_data);

        // Update channels to reflect what we actually have
        channels = actual_channels;

        return true;
    }

    bool apply(float r[], float g[], float b[], loader *loader, sdl_state &sdl_vstate)
    {

        std::vector<unsigned char> mask(width * height, 0);

        static unsigned int idx = 0;

        for (int i = 0; i < width; ++i)
        {
            for (int j = 0; j < height; ++j)
            {
                int index = (i * width + j) * actual_channels;

                if (data[index + 0] >= b[0] && data[index + 0] <= b[1])
                    mask[i * width + j] = 255;
                else
                    mask[i * width + j] = 0;

                if (data[index + 1] >= g[0] && data[index + 1] <= g[1])
                    mask[i * width + j] = 255;
                else
                    mask[i * width + j] = 0;

                if (data[index + 2] >= r[0] && data[index + 2] <= r[1])
                    mask[i * width + j] = 255;
                else
                    mask[i * width + j] = 0;
            }
        }

        // Save the binary mask as a grayscale PNG
        std::string output = "../assets/rgb_threshold_mask_";
        output.append(std::to_string(idx++));
        output.append(".png");

        stbi_write_png(output.c_str(), width, height, 1, mask.data(), width);

        loader->texture_load(output.c_str(), sdl_vstate.renderer, &sdl_vstate.src);

        return true;
    }

} binary_thresholds;