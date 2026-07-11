/*
** EPITECH PROJECT, 2025
** G-OOP-400-NCE-4-1-raytracer-8
** File description:
** SoType.hpp
*/

#ifndef SOTYPE_HPP
    #define SOTYPE_HPP

/*
** Each .so need a extern C function that return a SoTypeEnum in fuction of the object
** and an other extern C function name getObject/Light/Texture that return a new instance of the object
** *IObject, *ILight, *ITexture
** the cretor must take a std::map<std::string, std::string> with the first string the name of the varriable and the seconde the value.
** for param in list like { x = 60; y = 5; z = 40; r = 25; color = { r = 255; g = 64; b = 64; }; } to access r in color the name is color.r
*/

namespace raytracer {

typedef enum {
    OBJECT,
    LIGHT,
    Texture
} SoTypeEnum;

}

#endif