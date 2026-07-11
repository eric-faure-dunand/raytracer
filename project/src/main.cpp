/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** main.cpp
*/

#include "main.hpp"

int main(int argc, char **argv) {
    raytracer::Argument arg(argc, argv);

    try {
        arg.Test();
    } catch (const IError& e) {
        if (e.code() == 84) {
            std::cerr << e;
            return 84;
        }
        std::cout << e.what();
        return 0;
    }
    raytracer::CoreBuilder builder;
    builder.SetMode(arg.GetType()).SetSceneFile(arg.GetSceneFile());
    if (arg.GetType() == raytracer::SERVER)
        builder.SetPort(static_cast<std::uint16_t>(arg.GetPort()));
    if (arg.GetType() == raytracer::CLIENT)
        builder.SetClientConnection(arg.GetIp(), static_cast<std::uint16_t>(arg.GetPort())).SetNoInit(true);

    try {
        raytracer::Core core = builder.Build();
        core.Run();
    } catch (const IError& e) {
        if (e.code() == 84) {
            std::cerr << e;
            return 84;
        }
        std::cout << e;
        return 0;
    }

    return 0;
}