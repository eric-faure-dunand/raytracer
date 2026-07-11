/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** Builder.hpp
*/

#ifndef BUILDER_HPP
    #define BUILDER_HPP
    #include <cstdint>
    #include <memory>
    #include <string>

    #include "Core.hpp"
    #include "args.hpp"
    #include "CfgReader.hpp"
    #include "IManager.hpp"
    #include "DefaultManager.hpp"
    #include "ServerManager.hpp"
    #include "ClientManager.hpp"

namespace raytracer {

class CoreBuilder {
    raytracer_type _mode = DEFAULT;
    std::string _sceneFile;
    std::string _ip;
    std::uint16_t _port = 0;
    bool _noInit = 0;

public:
    CoreBuilder& SetMode(raytracer_type mode);
    CoreBuilder& SetSceneFile(const std::string& sceneFile);
    CoreBuilder& SetClientConnection(const std::string& ip, std::uint16_t port);
    CoreBuilder& SetPort(std::uint16_t port);
    CoreBuilder& SetNoInit(bool NoInit) {_noInit = NoInit; return *this;};

    std::unique_ptr<IManager> BuildManager() const;
    std::unique_ptr<IReader> BuildReader() const;
    Core Build() const;
};

}

#endif
