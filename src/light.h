#include "math_utils.h"

class PointLight {
public:
    PointLight() = default;
    ~PointLight() = default;

private:
  Vec3 position;
  Vec3 color;
};