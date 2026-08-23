
#ifndef IMANAGER_HPP
    #define IMANAGER_HPP

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
    virtual ~IManager() = default;

    virtual void InitCore(Core& core) = 0;
    virtual State GetState(void) = 0;
};

}

#endif
