
#include "Core.hpp"

#include <iostream>

#include "Error.hpp"
#include "Warning.hpp"
#include "printer.hpp"

namespace raytracer {

Core::Core(const std::string& sceneFile)
    : _sceneFile(sceneFile) {
    Init();
}

void Core::Init() {
    if (!_sceneFile.empty()) {
        ReaderBuilder builder;
        auto reader = builder.SetSceneFile(_sceneFile).BuildReader();
        _cam.position =reader->GetCameraPosition();
        _cam.rotation = reader->GetCameraRotation();
        _cam.fieldOfView = reader->GetCameraFieldOfView();
    }
    std::cout << Color::CYAN << "Core ready." << Color::RESET << std::endl;
}

void Core::Run() {
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
