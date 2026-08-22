
#ifndef CORE_HPP
    #define CORE_HPP
    #include <array>
    #include <memory>
    #include <string>
    #include <vector>
    #include <chrono>

    #include "Builder.hpp"
    #include "IReader.hpp"
    #include "Camera.hpp"
    #include "UI.hpp"

namespace raytracer {

class IManager;

class Core {
    uint8_t _fps = 60;

    std::string _sceneFile;
    std::unique_ptr<Renderer> _renderer;

    std::unique_ptr<UI> _ui;

public:

    Core(const std::string& sceneFile);
    ~Core() {_renderer.reset();};

    void Run();
    void Init();
};

}

#endif
