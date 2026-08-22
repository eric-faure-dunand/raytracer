
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
    #include "Display.hpp"

namespace raytracer {

class IManager;

class Core {
    uint8_t _fps = 60;

    std::string _sceneFile;

    Camera _cam;

public:

    Core(const std::string& sceneFile);
    ~Core() = default;

    void Run();
    void Init();
};

}

#endif
