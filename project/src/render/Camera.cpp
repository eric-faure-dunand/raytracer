#include "Camera.hpp"

namespace raytracer::render {

Camera::Camera()
    = default;

Camera::Camera(const std::array<int, 3>& cameraPosition,
    const std::array<int, 3>& cameraRotation,
    std::pair<std::size_t, std::size_t> cameraResolution,
    double cameraFieldOfView)
    : position(cameraPosition)
    , rotation(cameraRotation)
    , resolution(cameraResolution)
    , fieldOfView(cameraFieldOfView) 
    {}

}
