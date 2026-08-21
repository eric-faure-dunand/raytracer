
#include "Core.hpp"

#include <iostream>

#include "Error.hpp"
#include "Warning.hpp"
#include "printer.hpp"

namespace raytracer {

Core::Core(std::unique_ptr<IReader> reader, std::unique_ptr<IManager> manager, const std::string& sceneFile, bool NoInit)
    : _manager(std::move(manager)), _reader(std::move(reader)), _sceneFile(sceneFile) {
    _init = NoInit ? false : true;
    if (NoInit)
        return;
    Init();
}

void Core::Init() {
    if (_reader) {
        _cam.position =_reader->GetCameraPosition();
        _cam.rotation = _reader->GetCameraRotation();
        _cam.fieldOfView = _reader->GetCameraFieldOfView();
        try {
            objects = _reader->GetObjects();
            lights = _reader->GetLights();
        } catch (const IError& e) {
            if (e.code() == 84)
                throw Error("Core : " + static_cast<std::string>(e.what()));
        }
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
    objects.clear();
    lights.clear();
    if (!_reader)
        return;
    try {
        objects = _reader->GetObjects();
        lights = _reader->GetLights();
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
    auto next_time = std::chrono::steady_clock::now() + std::chrono::duration<double>(1.0 / _fps);

    Display display(1280, 720, "Raytracer");
    while (display.isOpen()) {
        if (next_time <= std::chrono::steady_clock::now()) {
            display.beginFrame();
            _cam.UpdateVector();
            display.drawEditor(_cam);
            display.endFrame();

            next_time = std::chrono::steady_clock::now() + std::chrono::duration<double>(1.0 / _fps);
        }
    }
}

}
