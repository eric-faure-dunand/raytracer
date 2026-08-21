
#ifndef ATEXTURE_HPP
    #define ATEXTURE_HPP
    #include <array>

    #include "ITexture.hpp"

namespace raytracer {

class ATexture : public ITexture {
    std::array<int, 4> color;
    double reflextion;
public:
    ATexture(std::array<int, 4> color, double reflextion): color(color), reflextion(reflextion) {};
    ~ATexture() {};

    //geters for maths
};

}

#endif
