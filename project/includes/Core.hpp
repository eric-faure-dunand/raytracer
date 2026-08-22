
#ifndef CORE_HPP
    #define CORE_HPP
    #include <array>
    #include <memory>
    #include <string>
    #include <vector>
    #include <chrono>

    #include "IReader.hpp"
    #include "Camera.hpp"
    #include "Display.hpp"

namespace raytracer {

class IManager;

class Core {
    uint8_t _fps = 60;

    std::unique_ptr<IReader> _reader;
    std::string _sceneFile;

    Camera _cam;

public:

    Core(std::unique_ptr<IReader> reader, const std::string& sceneFile);
    ~Core() = default;

    void Run();
    void Init();
    void SetReader(std::unique_ptr<IReader> reader);
    IReader& GetReader();
};

}

#endif
