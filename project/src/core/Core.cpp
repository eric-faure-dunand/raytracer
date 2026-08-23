
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
    _ui = std::make_unique<UI>(1280, 720, "Raytracer");
    _renderer = std::make_unique<Renderer>();
    if (!_sceneFile.empty()) {
        ReaderBuilder builder;
        auto reader = builder.SetSceneFile(_sceneFile).BuildReader();
        _renderer->_scene._cam.position = reader->GetCameraPosition();
        _renderer->_scene._cam.rotation = reader->GetCameraRotation();
        _renderer->_scene._cam.fieldOfView = reader->GetCameraFieldOfView();
    }
    std::cout << Color::CYAN << "Core ready." << Color::RESET << std::endl;
}

void Core::Run() {
    auto next_time = std::chrono::steady_clock::now() + std::chrono::duration<double>(1.0 / _fps);

    while (_ui->isOpen()) {
        if (next_time <= std::chrono::steady_clock::now()) {
            _ui->beginFrame();
            _renderer->_scene._cam.UpdateVector();
            _ui->drawEditor(_renderer);
            _ui->endFrame();
            _ui->event(_renderer, _fps);

            next_time = std::chrono::steady_clock::now() + std::chrono::duration<double>(1.0 / _fps);
        }
    }
}

}
