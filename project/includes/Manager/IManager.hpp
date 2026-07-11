/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** IManager.hpp
*/

#ifndef IMANAGER_HPP
    #define IMANAGER_HPP
    #include <vector>

    #include "Core.hpp"
    #include "Camera.hpp"
    #include "Tile.hpp"
    #include "IObject.hpp"
    #include "ILight.hpp"

namespace raytracer {

typedef enum {
    WORKING,
    FINISH,
    WAIT,
    ASK,
} State;

class Core;
class IManager {
public:
    ~IManager() = default;

    virtual void InitCore(Core& core) = 0;
    virtual void Update(const std::vector<std::unique_ptr<IObject>>& objects, const std::vector<std::unique_ptr<ILight>>& lights, const render::Camera& camera, std::vector<std::vector<Tile>>& map) = 0;
    virtual State GetState(void) = 0;
};

}

#endif