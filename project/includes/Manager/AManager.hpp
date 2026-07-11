
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
    void Update(Scene& scene, const std::array<int, 3> BgCollor) = 0;
    State GetState(void) {return _state;};
};

}

#endif