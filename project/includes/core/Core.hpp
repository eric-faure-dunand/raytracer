/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** Core.hpp
*/

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
    #include "Tile.hpp"
    #include "IReader.hpp"
    #include "IObject.hpp"
    #include "ILight.hpp"
    #include "Camera.hpp"
    #include "ppm.hpp"
    #include "SfmlDisplay.hpp"

namespace raytracer {

class IManager;

class Core {
    bool _init;

    std::size_t _x;
    std::size_t _y;
    std::unique_ptr<IManager> _manager;
    std::unique_ptr<IReader> _reader;
    std::string _sceneFile;

    ToPpm ppmconvertor;
    std::optional<SfmlDisplay> sfml;
    render::Camera _camera;

    std::vector<std::unique_ptr<IObject>> Objects;
    std::vector<std::unique_ptr<ILight>> Lights;

public:
    std::vector<std::vector<Tile>> map;

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