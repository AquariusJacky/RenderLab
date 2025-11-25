#include "camera.h"
#include "math_utils.h"
#include "ppmwriter.h"
#include "scene.h"

#define PI 3.14159f

#define SCENE_W 1920
#define SCENE_H 1080

int main() {
  int width = SCENE_W;
  int height = SCENE_H;

  PPMWriter img(width, height);
  Camera cam(Vec3(0, 0, 0),                // position
             Vec3(0, 0, -1),               // lookAt
             Vec3(0, 1, 0),                // up
             PI / 4.0f,                    // fov
             (float)width / (float)height  // aspect ratio
  );

  Scene scene;

  // x to the right, y up, -z into screen
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      float u = (float(x) + 0.5f) / float(width);
      float v = (float(y) + 0.5f) / float(height);

      Ray ray = cam.getRay(u, v);

      // Simple gradient based on ray direction
      Vec3 color = scene.getPixelColor(ray);

      float r = std::clamp(color.x, 0.0f, 1.0f);
      float g = std::clamp(color.y, 0.0f, 1.0f);
      float b = std::clamp(color.z, 0.0f, 1.0f);

      img.setPixel(x, y, static_cast<unsigned char>(r * 255),
                   static_cast<unsigned char>(g * 255),
                   static_cast<unsigned char>(b * 255));
    }
  }

  img.write("output.ppm");
  return 0;
}