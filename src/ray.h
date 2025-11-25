#include "math_utils.h"

class Ray {
public:
    Ray() = default;
    Ray(const Vec3& origin, const Vec3& direction)
        : origin(origin), direction(direction.normalized()) {}
    
    Vec3 getOrigin() const { return origin; }
    Vec3 getDirection() const { return direction; }
    
    Vec3 at(float t) const {
        return origin + direction * t;
    }

private:
    Vec3 origin;
    Vec3 direction;
};