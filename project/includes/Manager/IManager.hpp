
#ifndef IMANAGER_HPP
    #define IMANAGER_HPP
    #include <vector>

    #include "Core.hpp"
    #include "Camera.hpp"
    #include "Tile.hpp"
    #include "IObject.hpp"
    #include "ILight.hpp"
    #include "Scene.hpp"

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
    virtual void Update(Scene& scene, const std::array<int, 3> BgCollor) = 0;
    virtual State GetState(void) = 0;
};

}

#endif