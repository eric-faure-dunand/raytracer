/*
** EPITECH PROJECT, 2025
** G-OOP-400-NCE-4-1-raytracer-8
** File description:
** Builder.cpp
*/

#include "Builder.hpp"

namespace raytracer {

CoreBuilder& CoreBuilder::SetMode(raytracer_type mode) {
    _mode = mode;
    return *this;
}

CoreBuilder& CoreBuilder::SetSceneFile(const std::string& sceneFile) {
    _sceneFile = sceneFile;
    return *this;
}

CoreBuilder& CoreBuilder::SetClientConnection(const std::string& ip, std::uint16_t port) {
    _ip = ip;
    _port = port;
    return *this;
}

CoreBuilder& CoreBuilder::SetPort(std::uint16_t port) {
    _port = port;
    return *this;
}

std::unique_ptr<IManager> CoreBuilder::BuildManager() const {
    if (_mode == SERVER)
        return std::make_unique<ServerManager>(static_cast<int>(_port), _sceneFile);
    if (_mode == CLIENT)
        return std::make_unique<ClientManager>(_ip, _port);
    return std::make_unique<DefaultManager>();
}

std::unique_ptr<IReader> CoreBuilder::BuildReader() const {
    if (_mode == CLIENT)
        return nullptr;

    if (_sceneFile.empty())
        return nullptr;

    auto pos = _sceneFile.rfind('.');
    if (pos == std::string::npos)
        return nullptr;

    std::string ext = _sceneFile.substr(pos);
    if (ext == ".cfg")
        return std::make_unique<CfgReader>(_sceneFile);
    if (ext == ".obj")
        return nullptr;

    return nullptr;
}

Core CoreBuilder::Build() const {
    return Core(BuildReader(), BuildManager(), _sceneFile, _noInit);
}

}
