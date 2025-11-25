#include <vector>

#include "math_utils.h"
#include "shape.h"

class Scene {
 public:
  Scene() {
    // Initialize with some default objects

    spheres.push_back(Sphere(Vec3(0.0f, -1.0f, -5.0f), 1.0f));
    spheres.push_back(Sphere(Vec3(2.0f, 0.0f, -6.0f), 1.5f));
  }
  ~Scene() = default;

  Vec3 getPixelColor(const Ray& ray) {
    HitRecord closestHit;
    closestHit.t = std::numeric_limits<float>::max();

    // Check intersection with all spheres
    for (size_t i = 0; i < spheres.size(); i++) {
      HitRecord rec = intersect_sphere(ray, spheres[i]);
      // Process hit record to determine color
      if (rec.t < closestHit.t && rec.frontFace) {
        closestHit = rec;
      }
    }

    // If no hit, return background color
    if (closestHit.t == std::numeric_limits<float>::max()) {
      return Vec3(0.5f, 0.7f, 1.0f);  // Sky blue
    }
    return Vec3(1, 0, 0);
  }

 private:
  std::vector<Sphere> spheres;  // x, y, z, radius
};