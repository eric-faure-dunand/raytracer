
#ifndef DEFAULTMANAGER_HPP
    #define DEFAULTMANAGER_HPP
    #include "AManager.hpp"

namespace raytracer {

// Mode local : pas de reseau. Le rendu est desormais fait par le GPU,
// ce manager ne calcule plus rien lui-meme.
class DefaultManager : public AManager {
public:
    DefaultManager() = default;
    ~DefaultManager() = default;
};

}

#endif
