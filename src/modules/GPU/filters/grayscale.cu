__global__ void rgb_to_grayscale(unsigned char *input, unsigned char *output, int width, int height)
{
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;

    if (x >= width || y >= height)
        return; // Boundary check

    int idx = (y * width + x) * 3; // Base index for the pixel (RGB)

    unsigned char r = input[idx + 0];
    unsigned char g = input[idx + 1];
    unsigned char b = input[idx + 2];

    // Standard luminance formula
    unsigned char gray = (unsigned char)(0.299f * r + 0.587f * g + 0.114f * b);

    output[y * width + x] = gray; // Grayscale image: 1 byte per pixel
}