#ifndef CAMERA_H
#define CAMERA_H

#include "math_utils.h"
#include "ray.h"

class Camera {
 public:
  Camera(const Vec3& position, const Vec3& lookAt, const Vec3& up, float fov,
         float aspectRatio)
      : position(position), fov(fov), aspectRatio(aspectRatio) {
    Vec3 forward = (lookAt - position).normalized();
    Vec3 right = forward.cross(up).normalized();
    Vec3 trueUp = right.cross(forward);

    this->forward = forward;
    this->right = right;
    this->up = trueUp;
  }

  Ray getRay(float u, float v) const {
    float tanFov = std::tan(fov / 2.0f);

    // Mapping [0, 1] to [-tanFov * aspectRatio, tanFov * aspectRatio] for x
    float px = (2.0f * u - 1.0f) * tanFov * aspectRatio;

    // Mapping [0, 1] to [tanFov, -tanFov] for y
    // (Note the inversion of y to match image coordinates)
    float py = (1.0f - 2.0f * v) * tanFov;

    Vec3 rayDir = (forward + right * px + up * py).normalized();
    return Ray(position, rayDir);

    /*
                              Screen
                               .|\
                           .    |
                       .        | screen_height/2
                   .            |
               .                |/
    Camera . ------- d = 1------|--->  Ray Direction
               .                |
                   .            |
                       .        |
                           .    |
                               .|

    This is vertical FOV, so tan(fov / 2) = (screen_height / 2) / d
    */
  }

 private:
  Vec3 position;      // Center of camera
  Vec3 forward;       // Direction camera is facing
  Vec3 right;         // Right vector
  Vec3 up;            // Up vector
  float fov;          // in radians
  float aspectRatio;  // width / height
};

#endif  // CAMERA_H