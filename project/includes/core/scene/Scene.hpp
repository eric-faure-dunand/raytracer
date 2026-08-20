
#ifndef SCENE_HPP
    #define SCENE_HPP
    #include <vector>
    #include <memory>

    #include "Tile.hpp"
    #include "IObject.hpp"
    #include "ILight.hpp"
    #include "Camera.hpp"

namespace raytracer{
class Scene {
public:
    std::size_t _x;
    std::size_t _y;

    render::Camera _camera;

    std::vector<std::vector<Tile>> _screen;

    void InitScreen(void) {
        _screen.resize(_y);
            for (auto& row : _screen)
                row.assign(_x, Tile({0, 0, 0}));
    }

    Scene() {};
    Scene(std::size_t x, std::size_t y,
        render::Camera camera): 
        _x(x), _y(y),
        _camera(camera)
        {
            InitScreen();
        };
    ~Scene() = default;

    Scene& operator=(const Scene& other) {
        _x = other._x;
        _y = other._y;
        _camera = other._camera;
        InitScreen();
        return *this;
    };
};

class SceneBuilder {
    std::size_t _x = 800;
    std::size_t _y = 600;

    render::Camera _camera;
public:
    void add_x(std::size_t x) {_x = x;};
    void add_y(std::size_t y) {_y = y;};
    void add_camera(render::Camera camera) {_camera = camera;};

    Scene BuildScene() {
        return Scene(_x, _y, _camera);
    }
};


}

#endif