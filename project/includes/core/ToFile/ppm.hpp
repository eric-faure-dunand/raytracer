/*
** EPITECH PROJECT, 2025
** G-OOP-400-NCE-4-1-raytracer-8
** File description:
** ppm.hpp
*/

#ifndef PPM_HPP
    #define PPM_HPP
    #include <string>
    #include <vector>
    #include <fstream>

    #include "Tile.hpp"
    #include "Error.hpp"

namespace raytracer {

class ToPpm {
public:
    ToPpm() {};
    ~ToPpm() = default;

    void Draw(const std::string& path, const std::vector<std::vector<Tile>>& map) {
        std::ofstream file(path);
        if (!file.is_open())
            throw Error("Toppm : Cannot create / open" + path);
        if (map.empty() || map[0].empty())
            throw Error("Toppm : Empty map");
        int height = map.size();
        int width  = map[0].size();

        file << "P3\n";
        file << width << " " << height << "\n";
        file << "255\n";

        for (const auto& row : map) {
            for (const auto& tile : row)
                file << tile.GetRed() << " " << tile.GetGreen() << " " << tile.GetBlue() << " ";
            file << "\n";
        }
        file.close();
    }
};

}

#endif