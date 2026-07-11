#pragma once

#include <array>
#include <cstddef>
#include <utility>

namespace raytracer::render {

class Camera {
public:
    std::array<int, 3> position {0, 0, 0};
    std::array<int, 3> rotation {0, 0, 0};
    std::pair<std::size_t, std::size_t> resolution {0, 0};
    double fieldOfView = 60.0;

    Camera();
    Camera(const std::array<int, 3>& cameraPosition,
        const std::array<int, 3>& cameraRotation,
        std::pair<std::size_t, std::size_t> cameraResolution,
        double cameraFieldOfView);

    Camera(const Camera& other) {
        position = other.position;
        rotation = other.rotation;
        resolution = other.resolution;
        fieldOfView = other.fieldOfView;
    }

    Camera& operator=(const Camera& other) {
        position = other.position;
        rotation = other.rotation;
        resolution = other.resolution;
        fieldOfView = other.fieldOfView;
        return *this;
    };
};

}
