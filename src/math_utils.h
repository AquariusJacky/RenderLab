#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <cmath>
#include <algorithm>

// Vec3 - for 3D positions, colors, directions
struct Vec3 {
    float x, y, z;
    
    Vec3() : x(0), y(0), z(0) {}
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
    Vec3(float v) : x(v), y(v), z(v) {}
    
    Vec3 operator+(const Vec3& v) const { return Vec3(x + v.x, y + v.y, z + v.z); }
    Vec3 operator-(const Vec3& v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
    Vec3 operator*(float s) const { return Vec3(x * s, y * s, z * s); }
    Vec3 operator/(float s) const { return Vec3(x / s, y / s, z / s); }
    Vec3 operator*(const Vec3& v) const { return Vec3(x * v.x, y * v.y, z * v.z); }
    
    Vec3& operator+=(const Vec3& v) { x += v.x; y += v.y; z += v.z; return *this; }
    Vec3& operator-=(const Vec3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
    Vec3& operator*=(float s) { x *= s; y *= s; z *= s; return *this; }
    Vec3& operator/=(float s) { x /= s; y /= s; z /= s; return *this; }
    
    Vec3 operator-() const { return Vec3(-x, -y, -z); }
    
    float dot(const Vec3& v) const { return x * v.x + y * v.y + z * v.z; }
    
    Vec3 cross(const Vec3& v) const {
        return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }
    
    float length() const { return std::sqrt(x * x + y * y + z * z); }
    float lengthSquared() const { return x * x + y * y + z * z; }
    
    Vec3 normalized() const {
        float len = length();
        return len > 0 ? *this / len : Vec3(0, 0, 0);
    }
    
    Vec3& normalize() {
        float len = length();
        if (len > 0) *this /= len;
        return *this;
    }

    Vec3 reflect(const Vec3& normal) const {
        return *this - normal * 2.0f * (*this).dot(normal);
    }
};

inline Vec3 operator*(float s, const Vec3& v) { return v * s; }
inline Vec3 max(const Vec3& a, const Vec3& b) {
    return Vec3(
        std::max(a.x, b.x),
        std::max(a.y, b.y),
        std::max(a.z, b.z)
    );
}
inline Vec3 min(const Vec3& a, const Vec3& b) {
    return Vec3(
        std::min(a.x, b.x),
        std::min(a.y, b.y),
        std::min(a.z, b.z)
    );
}

// Vec4 - for homogeneous coordinates
struct Vec4 {
    float x, y, z, w;
    
    Vec4() : x(0), y(0), z(0), w(0) {}
    Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    Vec4(const Vec3& v, float w) : x(v.x), y(v.y), z(v.z), w(w) {}
    
    Vec3 xyz() const { return Vec3(x, y, z); }
    Vec3 toVec3() const { return w != 0 ? Vec3(x/w, y/w, z/w) : Vec3(x, y, z); }
    
    Vec4 operator+(const Vec4& v) const { return Vec4(x + v.x, y + v.y, z + v.z, w + v.w); }
    Vec4 operator-(const Vec4& v) const { return Vec4(x - v.x, y - v.y, z - v.z, w - v.w); }
    Vec4 operator*(float s) const { return Vec4(x * s, y * s, z * s, w * s); }
    Vec4 operator/(float s) const { return Vec4(x / s, y / s, z / s, w / s); }
    
    float dot(const Vec4& v) const { return x * v.x + y * v.y + z * v.z + w * v.w; }
};

// Mat4 - 4x4 matrix for transformations
struct Mat4 {
    float m[16]; // Column-major order
    
    Mat4() {
        for (int i = 0; i < 16; i++) m[i] = 0;
    }
    
    static Mat4 identity() {
        Mat4 mat;
        mat.m[0] = mat.m[5] = mat.m[10] = mat.m[15] = 1.0f;
        return mat;
    }
    
    static Mat4 translation(float x, float y, float z) {
        Mat4 mat = identity();
        mat.m[12] = x; mat.m[13] = y; mat.m[14] = z;
        return mat;
    }
    
    static Mat4 scale(float x, float y, float z) {
        Mat4 mat = identity();
        mat.m[0] = x; mat.m[5] = y; mat.m[10] = z;
        return mat;
    }
    
    static Mat4 rotationX(float angle) {
        Mat4 mat = identity();
        float c = std::cos(angle), s = std::sin(angle);
        mat.m[5] = c; mat.m[6] = s;
        mat.m[9] = -s; mat.m[10] = c;
        return mat;
    }
    
    static Mat4 rotationY(float angle) {
        Mat4 mat = identity();
        float c = std::cos(angle), s = std::sin(angle);
        mat.m[0] = c; mat.m[2] = -s;
        mat.m[8] = s; mat.m[10] = c;
        return mat;
    }
    
    static Mat4 rotationZ(float angle) {
        Mat4 mat = identity();
        float c = std::cos(angle), s = std::sin(angle);
        mat.m[0] = c; mat.m[1] = s;
        mat.m[4] = -s; mat.m[5] = c;
        return mat;
    }
    
    Mat4 operator*(const Mat4& other) const {
        Mat4 result;
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                float sum = 0;
                for (int k = 0; k < 4; k++) {
                    sum += m[k * 4 + row] * other.m[col * 4 + k];
                }
                result.m[col * 4 + row] = sum;
            }
        }
        return result;
    }
    
    Vec4 operator*(const Vec4& v) const {
        return Vec4(
            m[0] * v.x + m[4] * v.y + m[8] * v.z + m[12] * v.w,
            m[1] * v.x + m[5] * v.y + m[9] * v.z + m[13] * v.w,
            m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14] * v.w,
            m[3] * v.x + m[7] * v.y + m[11] * v.z + m[15] * v.w
        );
    }
    
    Vec3 transformPoint(const Vec3& v) const {
        Vec4 result = *this * Vec4(v, 1.0f);
        return result.toVec3();
    }
    
    Vec3 transformVector(const Vec3& v) const {
        Vec4 result = *this * Vec4(v, 0.0f);
        return result.xyz();
    }
};

inline Vec3 lerp(const Vec3& a, const Vec3& b, float t) {
    return a * (1.0f - t) + b * t;
}

#endif // MATH_UTILS_H