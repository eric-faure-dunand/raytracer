
#ifndef AMANAGER_HPP
    #define AMANAGER_HPP
    #include "IManager.hpp"

namespace raytracer {

class AManager : public IManager {
protected:
    State _state = WAIT;

public:
    AManager() = default;
    ~AManager() = default;

    void InitCore(Core& core) override { (void)core; }
    State GetState(void) override { return _state; }
};

}

#endif
