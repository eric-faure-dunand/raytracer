#ifndef CAMERA_HPP
    #define CAMERA_HPP
    #include <cstddef>
    #include <utility>
    #include <cmath>
    #include <algorithm>

    #include "MyVector.hpp"

namespace raytracer {

class Camera {
public:
    Vector3 position {0, 0, 0};
    Vector3 rotation {0, 0, -1};
    Vector3 right {1, 0, 0};
    Vector3 up {0, 1, 0};
    double fieldOfView = 60.0;
    float yaw   = -static_cast<float>(M_PI) / 2.0f;
    float pitch = 0.0f;

    Camera() = default;
    Camera(const Vector3& cameraPosition,
        const Vector3& cameraRotation,
        double cameraFieldOfView)
    : position(cameraPosition)
    , rotation(cameraRotation)
    , fieldOfView(cameraFieldOfView)
    {};

    Camera(const Camera& other) {
        position = other.position;
        rotation = other.rotation;
        fieldOfView = other.fieldOfView;
        right = other.right;
        up = other.up;
        fieldOfView = other.fieldOfView;
    }

    Camera& operator=(const Camera& other) {
        position = other.position;
        rotation = other.rotation;
        fieldOfView = other.fieldOfView;
        right = other.right;
        up = other.up;
        fieldOfView = other.fieldOfView;
        return *this;
    };

    void UpdateVector(void) {
    float rlen = std::sqrt(rotation.x * rotation.x + rotation.y * rotation.y + rotation.z * rotation.z);
        if (rlen > 1e-6f) {
            rotation.x /= rlen;
            rotation.y /= rlen;
            rotation.z /= rlen;
        }

        right.x = (- (rotation.z * 1.0f));
        right.y = 0.0f;
        right.z = (rotation.x * 1.0f);
        float len = std::sqrt(right.x * right.x + right.y * right.y + right.z * right.z);
        if (len > 1e-6f) {
            right.x /= len;
            right.y /= len;
            right.z /= len;
        } else
            right = {1.0f, 0.0f, 0.0f};

        up.x = (right.y * rotation.z) - (right.z * rotation.y);
        up.y = (right.z * rotation.x) - (right.x * rotation.z);
        up.z = (right.x * rotation.y) - (right.y * rotation.x);
    };

    void SyncAnglesFromRotation() {
        pitch = std::asin(std::clamp(rotation.y, -1.0f, 1.0f));
        yaw   = std::atan2(rotation.z, rotation.x);
    }

    void ApplyYawPitch(float deltaYaw, float deltaPitch) {
        constexpr float kMaxPitch = 1.55334f;

        yaw   += deltaYaw;
        pitch  = std::clamp(pitch + deltaPitch, -kMaxPitch, kMaxPitch);

        rotation.x = std::cos(pitch) * std::cos(yaw);
        rotation.y = std::sin(pitch);
        rotation.z = std::cos(pitch) * std::sin(yaw);
    }

};

}
#endif
