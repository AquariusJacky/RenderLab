#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "light.h"
#include "math_utils.h"
#include "ray.h"
#include "shading.h"
#include "shape.h"

class Scene {
public:
    Scene() = default;
    ~Scene() = default;

    Vec3 getPixelColor(const Ray& ray) {
        HitRecord closestHit;
        closestHit.t = std::numeric_limits<float>::max();

        // Check intersection with all spheres
        size_t hitID = -1;
        for (size_t i = 0; i < shapes.size(); i++) {
            HitRecord rec = shapes[i].intersect(ray);
            // Process hit record to determine color
            if (rec.t < closestHit.t && rec.frontFace) {
                hitID = i;
                closestHit = rec;
            }
        }

        // If no hit, return background color
        if (hitID == -1) return Vec3(0, 0, 0);  // Sky blue

        return phongShading(closestHit, shapes[hitID].getColor(), ray, lights);
    }

    void addSphere(const Sphere& sphere) {
        shapes.push_back(Shape(Shape::ShapeType::SPHERE, sphere));
    }
    void addPlane(const Plane& plane) {
        shapes.push_back(Shape(Shape::ShapeType::PLANE, plane));
    }
    void addPointLight(const PointLight& pointlight) {
        lights.push_back(Light(Light::LightType::POINT, pointlight));
    }
    void addDirectionalLight(const DirectionalLight& directionallight) {
        lights.push_back(
            Light(Light::LightType::DIRECTIONAL, directionallight));
    }

private:
    std::vector<Light> lights;
    std::vector<Shape> shapes;
};

#endif  // SCENE_H