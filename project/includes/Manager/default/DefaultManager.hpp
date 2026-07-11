/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** DefaultManager.hpp
*/

#ifndef DEFAULTMANAGER_HPP
    #define DEFAULTMANAGER_HPP
    #include <string>

    #include "AManager.hpp"
    #include "printer.hpp"

namespace raytracer {

class DefaultManager : public AManager {
    std::size_t CalculatedRow = 0;
    std::size_t CalculingRow = 0;
    Printer _Printer;
public:
    DefaultManager() = default;
    ~DefaultManager() = default;

    void Update(const std::vector<std::unique_ptr<IObject>>& objects, const std::vector<std::unique_ptr<ILight>>& lights, const render::Camera& camera, std::vector<std::vector<Tile>>& map);
};

}

#endif