
#include "Core.hpp"

namespace raytracer {

Core::Core(std::unique_ptr<IReader> reader, std::unique_ptr<IManager> manager, const std::string& sceneFile, bool NoInit)
    : _x(0), _y(0), _manager(std::move(manager)), _reader(std::move(reader)), _sceneFile(sceneFile) {
    _init = NoInit ? false : true;
    if (NoInit)
        return;
    this->Init();
}

void Core::Init() {
    if (_reader) {
        std::pair<size_t, size_t> size = _reader->GetCameraResolution();
        _x = size.first;
        _y = size.second;
        _camera = render::Camera(
            _reader->GetCameraPosition(),
            _reader->GetCameraRotation(),
            size,
            _reader->GetCameraFieldOfView());
    }
    map.resize(_y);
    for (auto& row : map)
        row.assign(_x, Tile({0, 0, 0}));
    sfml.emplace(_x, _y);
    try {
        Objects = _reader->GetObjects();
        Lights  = _reader->GetLights();
    } catch (const IError& e) {
        if (e.code() == 84)
            throw Error("Core : " + static_cast<std::string>(e.what()));
    }
}

void Core::SetReader(std::unique_ptr<IReader> reader) {
    _reader = std::move(reader);
}

IReader& Core::GetReader() {
    if (!_reader)
        throw Warning("Core: reader is not set.");
    return *_reader;
}

void Core::ReloadObjectsAndLights() {
    Objects.clear();
    Lights.clear();
    if (!_reader)
        return;
    try {
        Objects = _reader->GetObjects();
        Lights = _reader->GetLights();
    } catch (const IError& e) {
        if (e.code() == 84)
            throw Error("Core : " + static_cast<std::string>(e.what()));
    }
}

void Core::MarkInitialized() {
    _init = true;
}

void Core::Run() {
    while (!_init)
        _manager->InitCore(*this);
    std::cout << Color::CYAN << "Core ready." << Color::RESET << std::endl;
    while (sfml->isOpen()) {
        if (!sfml->pollEvents())
            break;
        _manager->Update(Objects, Lights, _camera, map);
        sfml->render(map);
    }
    if (!_sceneFile.empty())
        ppmconvertor.Draw(_sceneFile.substr(0, _sceneFile.rfind('.')) + ".ppm", map);
}

}