#ifndef CAMERA_HPP
    #define CAMERA_HPP

    #include <array>
    #include <cstddef>
    #include <utility>
    #include <cmath>

namespace raytracer {

class Camera {
public:
    std::array<float, 3> position {0, 0, 0};
    std::array<float, 3> rotation {0, 0, -1};
    std::array<float, 3> right {1, 0, 0};
    std::array<float, 3> up {0, 1, 0};
    double fieldOfView = 60.0;

    Camera() = default;
    Camera(const std::array<float, 3>& cameraPosition,
        const std::array<float, 3>& cameraRotation,
        double cameraFieldOfView)
    : position(cameraPosition)
    , rotation(cameraRotation)
    , fieldOfView(cameraFieldOfView)
    {};

    Camera(const Camera& other) {
        position = other.position;
        rotation = other.rotation;
        fieldOfView = other.fieldOfView;
    }

    Camera& operator=(const Camera& other) {
        position = other.position;
        rotation = other.rotation;
        fieldOfView = other.fieldOfView;
        return *this;
    };

    void UpdateVector(void) {
        right[0] = - (rotation[2] * 1.0f);
        right[1] = 0.0f;
        right[2] = (rotation[0] * 1.0f);
        float len = std::sqrt(right[0]*right[0] + right[1]*right[1] + right[2]*right[2]);
        right[0] /= len;
        right[1] /= len;
        right[2] /= len;

        up[0] = (right[1] * rotation[2]) - (right[2] * rotation[1]);
        up[1] = (right[2] * rotation[0]) - (right[0] * rotation[2]);
        up[2] = (right[0] * rotation[1]) - (right[1] * rotation[0]);
    };
};

}
#endif
