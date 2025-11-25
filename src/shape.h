#include "math_utils.h"
#include "ray.h"

struct HitRecord {
  Vec3 point;
  Vec3 normal;
  float t;
  bool frontFace;  // true if ray hits front face

  inline void setFaceNormal(const Ray& r, const Vec3& outwardNormal) {
    frontFace = r.getDirection().dot(outwardNormal) < 0;
    normal = frontFace ? outwardNormal : -outwardNormal;
  }
};

struct Sphere {
  Vec3 center;
  float radius;

  Sphere(const Vec3& center, float radius) : center(center), radius(radius) {}
};

HitRecord intersect_sphere(const Ray& ray, const Sphere& sphere) {
  HitRecord rec;

  Vec3 oc = ray.getOrigin() - sphere.center;

  return rec;
}