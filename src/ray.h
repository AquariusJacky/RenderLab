#ifndef RAY_H
#define RAY_H

#include "math_utils.h"

class Ray {
public:
    Ray(const Vec3& origin, const Vec3& direction)
        : origin(origin), direction(direction.normalized()) {}
    
    Vec3 getOrigin() const { return origin; }
    Vec3 getDirection() const { return direction; }
    
    Vec3 at(float t) const {
        return origin + direction * t;
    }

    Vec3 reflect(const Vec3& normal) const {
        return direction - normal * 2.0f * direction.dot(normal);
    }

private:
    Vec3 origin;
    Vec3 direction;
};

#endif // RAY_H