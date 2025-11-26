#ifndef SHADING_H
#define SHADING_H

#include <algorithm>
#include <iostream>
#include <vector>

#include "light.h"
#include "math_utils.h"
#include "ray.h"
#include "shape.h"

Vec3 simpleShading(const HitRecord& rec, const Vec3& shape_color) {
  Vec3 N = rec.normal;

  return Vec3((N.x + 1) * 0.5f, (N.y + 1) * 0.5f, (N.z + 1) * 0.5f) *
         shape_color;
}

Vec3 phongShading(const HitRecord& rec, const Vec3& shape_color, const Ray& ray,
                  const std::vector<Light>& lights) {
  Vec3 N = rec.normal;

  // Ambient
  Vec3 ambient = shape_color * 0.1;

  Vec3 light_contribution(0, 0, 0);
  for (size_t i = 0; i < lights.size(); i++) {
    Light light = lights[i];
    if (light.type == Light::LightType::POINT) {
      // Diffuse
      Vec3 light_dir = light.pointLight.position -
                       rec.point;  // Direction from hit point to light
      float diff = (N.dot(light_dir) > 0.0f)
                       ? (N.dot(light_dir) / light_dir.length())
                       : 0.0f;
      Vec3 diffuse = shape_color * diff;

      // Specular
      Vec3 reflect_dir = light_dir.reflect(N);
      float dotProduct = ray.getDirection().dot(reflect_dir.normalized());
      float spec = std::pow((dotProduct > 0 ? dotProduct : 0), 32);
      Vec3 specular = shape_color * spec;

      light_contribution += (diffuse + specular);
    }
  }
  return ambient + light_contribution;
}

#endif  // SHADING_H