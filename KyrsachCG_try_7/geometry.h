#pragma once
#include <iostream>
#include <vector>
#include <cassert>

template <class t> struct Vec2 {
    t x, y;
    Vec2<t>() : x(t()), y(t()) {}
    Vec2<t>(t _x, t _y) : x(_x), y(_y) {}
    Vec2<t>(const Vec2<t>& v) : x(t()), y(t()) { *this = v; }
    Vec2<t>& operator =(const Vec2<t>& v) {
        if (this != &v) {
            x = v.x;
            y = v.y;
        }
        return *this;
    }
    Vec2<t> operator +(const Vec2<t>& V) const { return Vec2<t>(x + V.x, y + V.y); }
    Vec2<t> operator -(const Vec2<t>& V) const { return Vec2<t>(x - V.x, y - V.y); }
    Vec2<t> operator *(float f)          const { return Vec2<t>(x * f, y * f); }
    Vec2<t> operator /(float f) const {
        if (f != 0) {
            return Vec2<t>(x / f, y / f);
        }
        return Vec2<t>(0, 0);
    }
    t& operator[](const int i) { if (x <= 0) return x; else return y; }
    template <class > friend std::ostream& operator<<(std::ostream& s, Vec2<t>& v);
};

template <class t> struct Vec3 {
    t x, y, z;
    Vec3<t>() : x(t()), y(t()), z(t()) { }
    Vec3<t>(t _x, t _y, t _z) : x(_x), y(_y), z(_z) {}
    Vec3<t>(t _x, t _y) : x(_x), y(_y), z(0) {}
    template <class u> Vec3<t>(const Vec3<u>& v);
    Vec3<t>(const Vec3<t>& v) : x(t()), y(t()), z(t()) { *this = v; }
    Vec3<t>& operator =(const Vec3<t>& v) {
        if (this != &v) {
            x = v.x;
            y = v.y;
            z = v.z;
        }
        return *this;
    }
    Vec3<t> operator ^(const Vec3<t>& v) const { return Vec3<t>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }
    Vec3<t> operator +(const Vec3<t>& v) const { return Vec3<t>(x + v.x, y + v.y, z + v.z); }
    Vec3<t> operator -(const Vec3<t>& v) const { return Vec3<t>(x - v.x, y - v.y, z - v.z); }
    Vec3<t> operator *(float f)          const { return Vec3<t>(x * f, y * f, z * f); }
    Vec3<t> operator /(float f) const {
        if (f != 0) {
            return Vec3<t>(x / f, y / f, z / f);
        }
        return Vec3<t>(0, 0, 0);
    }
    t       operator *(const Vec3<t>& v) const { return x * v.x + y * v.y + z * v.z; }
    bool operator==(const Vec3<t>& v) const {
        return x == v.x && y == v.y;
    }
    bool operator!=(const Vec3<t>& v) const {
        return !(x == v.x && y == v.y && z == v.z);
    }
    float norm() const { return std::sqrt(x * x + y * y + z * z); }
    Vec3<t>& normalize(t l = 1) { *this = (*this) * (l / norm()); return *this; }
    t& operator[](const int i) { if (i <= 0) return x; else if (i == 1) return y; else return z; }
    template <class > friend std::ostream& operator<<(std::ostream& s, Vec3<t>& v);
};

typedef Vec2<double> Vec2d;
typedef Vec2<float>  Vec2f;
typedef Vec2<int>    Vec2i;
typedef Vec3<double> Vec3d;
typedef Vec3<float>  Vec3f;
typedef Vec3<int>    Vec3i;

template <class t> struct Vec3Screen {
    t x, y;
    double z;
    Vec3Screen<t>() : x(t()), y(t()), z(double()) { }
    Vec3Screen<t>(t _x, t _y, double _z) : x(_x), y(_y), z(_z) {}
    Vec3Screen<t>(t _x, t _y) : x(_x), y(_y), z(0.0f) {}
    //template <class u> Vec3<t>(const Vec3<u>& v);
    //Vec3<t>(const Vec3<t>& v) : x(t()), y(t()), z(double()) { *this = v; }
    Vec3Screen<t>& operator =(const Vec3<t>& v) {
        if (this != &v) {
            x = v.x;
            y = v.y;
            z = v.z;
        }
        return *this;
    }
    Vec3Screen<t> operator ^(const Vec3Screen<t>& v) const { return Vec3Screen<t>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }
    Vec3Screen<t> operator +(const Vec3Screen<t>& v) const { return Vec3Screen<t>(x + v.x, y + v.y, z + v.z); }
    Vec3Screen<t> operator -(const Vec3Screen<t>& v) const { return Vec3Screen<t>(x - v.x, y - v.y, z - v.z); }
    Vec3Screen<t> operator *(float f)          const { return Vec3Screen<t>(x * f, y * f, z * f); }
    Vec3Screen<t> operator /(float f) const {
        if (f != 0) {
            return Vec3Screen<t>(x / f, y / f, z / f);
        }
        return Vec3Screen<t>(0, 0, 0);
    }
    t       operator *(const Vec3Screen<t>& v) const { return x * v.x + y * v.y + z * v.z; }
    bool operator==(const Vec3Screen<t>& v) const {
        return x == v.x && y == v.y;
    }
    bool operator!=(const Vec3Screen<t>& v) const {
        return !(x == v.x && y == v.y && z == v.z);
    }
    float norm() const { return std::sqrt(x * x + y * y + z * z); }
    Vec3Screen<t>& normalize(t l = 1) { *this = (*this) * (l / norm()); return *this; }
    t& operator[](const int i) { if (i <= 0) return x; else if (i == 1) return y; else return z; }
    //template <class > friend std::ostream& operator<<(std::ostream& s, Vec3Screen<t>& v);
};

typedef Vec3Screen<int>    Vec3s;

namespace geo
{
    struct Polygon 
    { 
        std::vector<Vec3i> points; 
        float intensity; 

        bool operator==(const Polygon& other) const
        {

            if (points.size() != other.points.size())
                return false;

            for (size_t i = 0; i < points.size(); ++i)
            {
                if (points[i] != other.points[i])
                    return false;
            }

            return intensity == other.intensity;
        }
    };
}


template <class T> struct Triangle {
    T vertex[3];
    T center;
    Vec3f normal;
    float intensity;
    Triangle<T>() {
        vertex[0] = T();
        vertex[1] = T();
        vertex[2] = T();
        center = T();
        normal = Vec3f();
        intensity = 0.0f;
    }
    Triangle<T>(T _vertex1, T _vertex2, T _vertex3, Vec3f _normal = Vec3f(), float _intensity = 0.0f) {
        vertex[0] = _vertex1;
        vertex[1] = _vertex2;
        vertex[2] = _vertex3;
        calculateCenter();
        normal = _normal;
        intensity = _intensity;
    }
    Triangle<T>(const Triangle<T>& other) {
        vertex[0] = other.vertex[0];
        vertex[1] = other.vertex[1];
        vertex[2] = other.vertex[2];
        center = other.center;
        normal = other.normal;
        intensity = other.intensity;
    }

    void calculateCenter() {
        center = T(
            (vertex[0].x + vertex[1].x + vertex[2].x) / 3.0,
            (vertex[0].y + vertex[1].y + vertex[2].y) / 3.0,
            (vertex[0].z + vertex[1].z + vertex[2].z) / 3.0
        );
    }

    bool contains(const T& point) const {
        float alpha = ((vertex[1].y - vertex[2].y) * (point.x - vertex[2].x) + (vertex[2].x - vertex[1].x) * (point.y - vertex[2].y)) /
            ((vertex[1].y - vertex[2].y) * (vertex[0].x - vertex[2].x) + (vertex[2].x - vertex[1].x) * (vertex[0].y - vertex[2].y));

        float beta = ((vertex[2].y - vertex[0].y) * (point.x - vertex[2].x) + (vertex[0].x - vertex[2].x) * (point.y - vertex[2].y)) /
            ((vertex[1].y - vertex[2].y) * (vertex[0].x - vertex[2].x) + (vertex[2].x - vertex[1].x) * (vertex[0].y - vertex[2].y));

        float gamma = 1.0f - alpha - beta;

        return alpha > 0 && beta > 0 && gamma > 0;
    }
};

struct rect
{
    Vec3i pos_;
    Vec3i size_;

    rect(const Vec3i& pos = { 0, 0 }, const Vec3i size = { 1, 1 });

    bool contains(const Vec3i p)
    {
        return !(p.x < pos_.x || p.y < pos_.y || p.x >= (pos_.x + size_.x) || p.y >= (pos_.y + size_.y));
    }

    bool contains(const Triangle<Vec3i>& triangle)
    {
        for (int i = 0; i < 3; ++i) {
            if (!contains(triangle.vertex[i])) {
                return false;
            }
        }
        return true;
    }

    bool isInside(const Triangle<Vec3i>& triangle) const {
        for (int i = 0; i < 4; ++i) {
            if (!triangle.contains(pos_ + Vec3i((i % 2) * size_.x, (i / 2) * size_.y))) {
                return false;
            }
        }
        return true;
    }
};

template <> template <> Vec3<int>::Vec3(const Vec3<float>& v);
template <> template <> Vec3<float>::Vec3(const Vec3<int>& v);

template <class t>
bool operator==(const Vec3<t>& lhs, const Vec3<t>& rhs) {
    return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z);
}

template <class t> std::ostream& operator<<(std::ostream& s, Vec2<t>& v) {
    s << "(" << v.x << ", " << v.y << ")\n";
    return s;
}

template <class t> std::ostream& operator<<(std::ostream& s, Vec3<t>& v) {
    s << "(" << v.x << ", " << v.y << ", " << v.z << ")\n";
    return s;
}