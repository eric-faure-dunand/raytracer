/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** AManager.hpp
*/

#ifndef AMANAGER_HPP
    #define AMANAGER_HPP
    #include "IManager.hpp"
    #include "MultiThread.hpp"
    #include "Net.hpp"
namespace raytracer {

class AManager : public IManager {
protected:
    NetLog _logger;
    State _state = WAIT;
    MultiThread _multiThread;
public:
    AManager() {};
    ~AManager() = default;

    void InitCore(Core& core) {(void)core; std::cout << "nn" << std::endl;};
    void Update(const std::vector<std::unique_ptr<IObject>>& objects, const std::vector<std::unique_ptr<ILight>>& lights, const render::Camera& camera, std::vector<std::vector<Tile>>& map) = 0;
    State GetState(void) {return _state;};
};

}

#endif