#ifndef LIGHT_H
#define LIGHT_H

#include "math_utils.h"

struct PointLight {
    Vec3 position;
    Vec3 color;
    PointLight(const Vec3& position, const Vec3& color)
        : position(position), color(color) {}
};

struct DirectionalLight {
    Vec3 direction;
    Vec3 color;
    DirectionalLight(const Vec3& direction, const Vec3& color)
        : direction(direction), color(color) {}
};

struct Light {
    enum LightType { POINT, DIRECTIONAL, SPOT };

    Light(const LightType& type, const PointLight& point_light)
        : type(type), pointLight(point_light) {}
    Light(const LightType& type, const DirectionalLight& directional_light)
        : type(type), directionalLight(directional_light) {}
    LightType type;
    union {
        PointLight pointLight;
        DirectionalLight directionalLight;
    };
};

#endif  // LIGHT_H