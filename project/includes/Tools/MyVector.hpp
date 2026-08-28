#ifndef MYVECTOR_HPP
    #define MYVECTOR_HPP
    #include <array>
    #include <ostream>
    #include <format>

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

    bool operator!=(const Vector3 other) {
        return (other.x != x || other.y != y || other.z != z);
    }


    Vector3& operator=(const Vector3 other) {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }


    Vector3& operator-=(const Vector3& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    Vector3& operator+=(const Vector3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

};

inline std::ostream& operator<<(std::ostream& stream, const Vector3& vector) {
    return stream << "{" << std::format("{:.2f}, {:.2f}, {:.2f}", vector.x, vector.y, vector.z) << "}";
}

}

#endif
