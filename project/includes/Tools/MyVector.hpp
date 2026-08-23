#ifndef MYVECTOR_HPP
    #define MYVECTOR_HPP
    #include <array>

namespace raytracer{

class Vector3 {
public:
    float x, y, z = 0.0f;

    Vector3() = default;
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    Vector3(const std::array<float, 3> arr) : x(arr[0]), y(arr[1]), z(arr[2]) {}
    Vector3(const Vector3& other) : x(other.x), y(other.y), z(other.z) {};
    ~Vector3() = default;

    bool operator==(const Vector3 other) {
        return (other.x == x && other.y == y && other.z == z);
    }

    bool operator==(const std::array<float, 3> arr) {
        return *this == Vector3(arr);
    }

    bool operator!=(const Vector3 other) {
        return (other.x != x || other.y != y || other.z != z);
    }

    bool operator!=(const std::array<float, 3> arr) {
        return *this != Vector3(arr);
    }

    Vector3& operator=(const Vector3 other) {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    Vector3& operator=(const std::array<float, 3> arr) {
        return *this = Vector3(arr);
    }

    Vector3& operator-=(const Vector3& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    Vector3& operator-=(const std::array<float, 3> arr) {
        return *this -= Vector3(arr);
    }

    Vector3& operator+=(const Vector3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Vector3& operator+=(const std::array<float, 3> arr) {
        return *this += Vector3(arr);
    }
};

}

#endif
