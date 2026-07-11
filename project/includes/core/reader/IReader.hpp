/*
** EPITECH PROJECT, 2025
** G-OOP-400-NCE-4-1-raytracer-8
** File description:
** IReader.hpp
*/

#ifndef IREADER_HPP
    #define IREADER_HPP
    #include <string>
    #include <utility>
    #include <array>
    #include <cstddef>
    #include <vector>
    #include <memory>

    #include "Error.hpp"
    #include "IObject.hpp"
    #include "ILight.hpp"

namespace raytracer {

    class IReader {
    public:
        virtual void SetUp() = 0;

        virtual std::pair<std::size_t, std::size_t> GetCameraResolution() = 0;
        virtual std::array<int, 3> GetCameraPosition() = 0;
        virtual std::array<int, 3> GetCameraRotation() = 0;
        virtual double GetCameraFieldOfView() = 0;

        virtual std::vector<std::unique_ptr<IObject>> GetObjects() = 0;
        virtual std::vector<std::unique_ptr<ILight>> GetLights() = 0;

        virtual std::vector<std::string> GetNoOpenFile() = 0;
    };

}

#endif