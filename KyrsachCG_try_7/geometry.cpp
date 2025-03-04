#include "geometry.h"

rect::rect(const Vec3i& pos, const Vec3i size) : pos_(pos), size_(size) {}

template <> template <> Vec3<int>::Vec3(const Vec3<float>& v) : x(int(v.x + .5)), y(int(v.y + .5)), z(int(v.z + .5)) {
}

template <> template <> Vec3<float>::Vec3(const Vec3<int>& v) : x(v.x), y(v.y), z(v.z) {
}
