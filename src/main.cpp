#include "ppmwriter.h"

int main() {
    PPMWriter img(400, 300);
    
    // Simple gradient
    for (int y = 0; y < img.getHeight(); y++) {
        for (int x = 0; x < img.getWidth(); x++) {
            int r = (255 * x) / img.getWidth();
            int g = (255 * y) / img.getHeight();
            int b = 128;
            img.setPixel(x, y, r, g, b);
        }
    }
    
    img.write("output.ppm");
    return 0;
}