/*
** EPITECH PROJECT, 2025
** G-OOP-400-NCE-4-1-raytracer-8
** File description:
** ATexture.hpp
*/

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
