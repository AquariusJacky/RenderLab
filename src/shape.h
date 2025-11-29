#ifndef SHAPE_H
#define SHAPE_H

#include "math_utils.h"
#include "ray.h"

struct HitRecord {
    Vec3 point;
    Vec3 normal;
    float t;
    bool frontFace;  // true if ray hits front face

    inline void setFaceNormal(const Ray& r, const Vec3& outwardNormal) {
        // If the dot product is negative, the ray hits the front face
        frontFace = r.getDirection().dot(outwardNormal) < 0;
        normal = frontFace ? outwardNormal : -outwardNormal;
    }
};

struct Sphere {
    Vec3 center;
    float radius;
    Vec3 color;

    Sphere(const Vec3& center, const float& radius, const Vec3& color)
        : center(center), radius(radius), color(color) {}
};

HitRecord intersectSphere(const Ray& ray, const Sphere& sphere) {
    HitRecord rec;
    rec.t = std::numeric_limits<float>::max();

    Vec3 oc = ray.getOrigin() - sphere.center;
    Vec3 raydir = ray.getDirection();
    Vec3 proj = raydir * (oc.dot(raydir) / raydir.lengthSquared());

    /*
        ^
        |      /\
        |  -> /  \
        |  A /    \         ->
        |   /      \       .B
        |  /    90 (\.
        | /    .
        |/___________________
       O

                     ->          ->
       projection of A  onto ray B  =

                      ->      ->    ->    ->      ->    ->
       ->             B       A  .  B     B       A  .  B    ->
       A . cos(θ)  * ---  =  --------- * ---  =  --------- * B
                     |B|        |B|      |B|      |B|.|B|
    */

    Vec3 closestPoint = ray.getOrigin() - proj;
    Vec3 centerToClosest = closestPoint - sphere.center;
    float dist2 = centerToClosest.lengthSquared();
    float radius2 = sphere.radius * sphere.radius;

    if (dist2 > radius2) return rec;  // No intersection

    // Compute intersection t values
    float thc = std::sqrt(radius2 - dist2);
    float t0 = (proj.length() - thc) / raydir.length();
    float t1 = (proj.length() + thc) / raydir.length();

    // Find the nearest t that is positive
    float t = t0;
    if (t0 < 0) t = t1;
    if (t1 < 0) return rec;  // Both t0 and t1 are negative

    rec.t = t;
    rec.point = ray.at(rec.t);
    Vec3 outwardNormal = (rec.point - sphere.center) / sphere.radius;
    rec.setFaceNormal(ray, outwardNormal);

    return rec;
}

struct Plane {
    Vec3 point;   // A point on the plane
    Vec3 normal;  // Normal vector
    Vec3 color;   // Color of the plane

    Plane(const Vec3& point, const Vec3& normal, const Vec3& color)
        : point(point), normal(normal.normalized()), color(color) {}
};

HitRecord intersectPlane(const Ray& ray, const Plane& plane) {
    HitRecord rec;
    Vec3 rayDir = ray.getDirection();
    // Distance from ray origin to plane along the normal direction
    // denom for denominator
    float denom = rayDir.dot(plane.normal);
    if (std::fabs(denom) < 1e-6) {
        // Ray is parallel to the plane
        return rec;
    }

    // The connection from ray origin to an aribitrary plane point
    Vec3 originToPlane = plane.point - ray.getOrigin();
    // dot product gives us the distance along the normal direction from plane
    // point to ray origin
    // Divided by denom to get not only t in terms of normal direction but also
    // in terms of ray direction
    float t = originToPlane.dot(plane.normal) / denom;
    if (t < 0) {
        // Plane is behind the ray
        return rec;
    }
    rec.t = t;
    rec.point = ray.at(rec.t);
    rec.setFaceNormal(ray, plane.normal);

    return rec;
}

struct Shape {
    enum ShapeType { SPHERE, PLANE };

    Shape(const ShapeType& type, const Sphere& sphere)
        : type(type), sphere(sphere) {}
    Shape(const ShapeType& type, const Plane& plane)
        : type(type), plane(plane) {}
    ShapeType type;
    union {
        Sphere sphere;
        Plane plane;
    };

    Vec3 getColor() const {
        switch (type) {
            case ShapeType::SPHERE:
                return sphere.color;
            case ShapeType::PLANE:
                return Vec3(0.8f, 0.8f, 0.8f);  // Default gray for planes
            default:
                return Vec3(1, 0, 1);  // Magenta for error
        }
    }

    HitRecord intersect(const Ray& ray) const {
        switch (type) {
            case ShapeType::SPHERE:
                return intersectSphere(ray, sphere);
            case ShapeType::PLANE:
                return intersectPlane(ray, plane);
            default:
                return HitRecord();  // Empty hit record
        }
    }
};

#endif  // SHAPE_H