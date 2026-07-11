
#ifndef CORE_HPP
    #define CORE_HPP
    #include <cstddef>
    #include <memory>
    #include <string>
    #include <utility>
    #include <vector>
    #include <libconfig.h++>
    #include <optional>

    #include "IManager.hpp"
    #include "Error.hpp"
    #include "Warning.hpp"
    #include "IReader.hpp"
    #include "IObject.hpp"
    #include "ILight.hpp"
    #include "Camera.hpp"
    #include "ppm.hpp"
    #include "SfmlDisplay.hpp"

    #include "Scene.hpp"

namespace raytracer {

class IManager;

class Core {
    bool _init;

    std::unique_ptr<IManager> _manager;
    std::unique_ptr<IReader> _reader;
    std::string _sceneFile;
    std::array<int, 3> _backgroundColor = {255, 0, 255};

    ToPpm ppmconvertor;
    std::optional<SfmlDisplay> sfml;

    Scene _scene;
    std::vector<Scene> _subScene;

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