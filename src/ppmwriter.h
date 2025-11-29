#ifndef PPMWRITER_H
#define PPMWRITER_H

#include <algorithm>
#include <fstream>
#include <string>
#include <vector>

#include "math_utils.h"  // For clamp function

class PPMWriter {
public:
    PPMWriter(int width, int height)
        : width(width), height(height), pixels(width * height, {0, 0, 0}) {}

    void setPixel(int x, int y, int r, int g, int b) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            int idx = y * width + x;
            pixels[idx] = {static_cast<unsigned char>(clamp(r, 0, 255)),
                           static_cast<unsigned char>(clamp(g, 0, 255)),
                           static_cast<unsigned char>(clamp(b, 0, 255))};
        }
    }

    void setPixel(int x, int y, unsigned char r, unsigned char g,
                  unsigned char b) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            int idx = y * width + x;
            pixels[idx] = {r, g, b};
        }
    }

    bool write(const std::string& filename) {
        std::ofstream file(filename, std::ios::binary);
        if (!file) return false;

        // PPM header
        file << "P6\n" << width << " " << height << "\n255\n";

        // Write pixel data
        for (const auto& pixel : pixels) {
            file.write(reinterpret_cast<const char*>(&pixel), 3);
        }

        return file.good();
    }

    void clear(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0) {
        std::fill(pixels.begin(), pixels.end(), RGB{r, g, b});
    }

    int getWidth() const { return width; }
    int getHeight() const { return height; }

private:
    struct RGB {
        unsigned char r, g, b;
    };

    int width, height;
    std::vector<RGB> pixels;
};

#endif  // PPMWRITER_H