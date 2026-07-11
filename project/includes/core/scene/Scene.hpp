
#ifndef SCENE_HPP
    #define SCENE_HPP
    #include <vector>
    #include <memory>

    #include "Tile.hpp"
    #include "IObject.hpp"
    #include "ILight.hpp"
    #include "Camera.hpp"

namespace raytracer
{
class Scene {
public:
    std::size_t _x;
    std::size_t _y;

    render::Camera _camera;

    std::vector<std::shared_ptr<IObject>> _objects;
    std::vector<std::shared_ptr<ILight>> _lights;

    std::vector<std::vector<Tile>> _screen;

    void InitScreen(void) {
        _screen.resize(_y);
            for (auto& row : _screen)
                row.assign(_x, Tile({0, 0, 0}));
    }

    Scene() {};
    Scene(std::size_t x, std::size_t y,
        render::Camera camera,
        std::vector<std::shared_ptr<IObject>> Objects,
        std::vector<std::shared_ptr<ILight>> Lights): 
        _x(x), _y(y),
        _camera(camera),
        _objects(Objects),
        _lights(Lights)
        {
            InitScreen();
        };
    ~Scene() = default;

    Scene& operator=(const Scene& other) {
        _x = other._x;
        _y = other._y;
        _camera = other._camera;
        _objects = other._objects;
        _lights = other._lights;
        InitScreen();
        return *this;
    };
};

class SceneBuilder {
    std::size_t _x = 800;
    std::size_t _y = 600;

    render::Camera _camera;

    std::vector<std::shared_ptr<IObject>> _objects = {0};
    std::vector<std::shared_ptr<ILight>> _lights = {0};
public:
    void add_x(std::size_t x) {_x = x;};
    void add_y(std::size_t y) {_y = y;};
    void add_camera(render::Camera camera) {_camera = camera;};
    void add_objects(std::vector<std::shared_ptr<IObject>> objects) {_objects = objects;};
    void add_lights(std::vector<std::shared_ptr<ILight>> lights) {_lights = lights;};

    Scene BuildScene() {
        return Scene(_x, _y, _camera, _objects, _lights);
    }
};


} // namespace raytracer

#endif