
#ifndef CORE_HPP
    #define CORE_HPP
    #include <array>
    #include <memory>
    #include <string>
    #include <vector>
    #include <chrono>

    #include "IManager.hpp"
    #include "IReader.hpp"
    #include "IObject.hpp"
    #include "ILight.hpp"
    #include "Camera.hpp"
    #include "Display.hpp"

namespace raytracer {

class IManager;

class Core {
    bool _init;

    uint8_t _fps = 60;

    std::unique_ptr<IManager> _manager;
    std::unique_ptr<IReader> _reader;
    std::string _sceneFile;

    std::vector<std::unique_ptr<IObject>> objects;
    std::vector<std::unique_ptr<ILight>> lights;

    Camera _cam;

public:

    Core(std::unique_ptr<IReader> reader, std::unique_ptr<IManager> manager, const std::string& sceneFile, bool NoInit);
    ~Core() = default;

    void Run();
    void Init();
    void SetReader(std::unique_ptr<IReader> reader);
    IReader& GetReader();
    void ReloadObjectsAndLights();
    void MarkInitialized();
};

}

#endif
