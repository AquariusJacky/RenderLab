#include "camera.h"
#include "math_utils.h"
#include "ppmwriter.h"
#include "scene.h"

#define PI 3.14159f

#define IMG_WIDTH 1920
#define IMG_HEIGHT 1080

int main() {
    int width = IMG_WIDTH;
    int height = IMG_HEIGHT;

    PPMWriter img(width, height);
    Camera cam(Vec3(0, 5, 0),                // position
               Vec3(0, 5, -1),               // lookAt
               Vec3(0, 1, 0),                // up
               PI / 4.0f,                    // fov
               (float)width / (float)height  // aspect ratio
    );

    Scene scene;

    // Initialize with some default objects
    scene.addSphere(
        Sphere(Vec3(0.0f, 1.0f, -20.0f), 1.0f, Vec3(0.88f, 0.64f, 0.47f)));
    scene.addSphere(
        Sphere(Vec3(2.0f, 1.5f, -18.0f), 1.5f, Vec3(0.39f, 0.50f, 0.76f)));

    // Floor
    scene.addPlane(Plane(Vec3(0, 0, 0), Vec3(0, 1, 0), Vec3(1, 1, 1)));

    // Walls
    scene.addPlane(
        Plane(Vec3(-5.0f, 0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(1, 1, 1)));
    scene.addPlane(Plane(Vec3(0, 0, -25), Vec3(0, 0, 1), Vec3(1, 1, 1)));
    scene.addPlane(
        Plane(Vec3(5.0f, 0.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(1, 1, 1)));

    // Ceiling
    scene.addPlane(
        Plane(Vec3(0.0f, 10.0f, 0.0f), Vec3(0.0f, -1.0f, 0.0f), Vec3(1, 1, 1)));

    scene.addPointLight(
        PointLight(Vec3(0.0f, 9.0f, -15.0f), Vec3(1.0f, 1.0f, 1.0f)));

    // x to the right, y up, -z into screen
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float u = (float(x) + 0.5f) / float(width);
            float v = (float(y) + 0.5f) / float(height);

            Ray ray = cam.getRay(u, v);

            // Simple gradient based on ray direction
            Vec3 color = scene.getPixelColor(ray);

            float r = clamp(color.x, 0.0f, 1.0f);
            float g = clamp(color.y, 0.0f, 1.0f);
            float b = clamp(color.z, 0.0f, 1.0f);

            img.setPixel(x, y, static_cast<unsigned char>(r * 255),
                         static_cast<unsigned char>(g * 255),
                         static_cast<unsigned char>(b * 255));
        }
    }

    img.write("output.ppm");
    return 0;
}