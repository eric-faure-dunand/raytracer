
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

    void Update(Scene& scene, const std::array<int, 3> BgCollor);
};

}

#endif