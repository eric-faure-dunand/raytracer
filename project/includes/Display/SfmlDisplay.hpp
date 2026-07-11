/*
** EPITECH PROJECT, 2025
** G-OOP-400-NCE-4-1-raytracer-8
** File description:
** SfmlDisplay.hpp
*/

#ifndef SFMLDISPLAY_HPP
    #define SFMLDISPLAY_HPP
    #include <cstddef>
    #include <vector>
    #include <stdexcept>
    #include <SFML/Graphics.hpp>

    #include "Tile.hpp"

namespace raytracer {

class SfmlDisplay {
    sf::RenderWindow _window;
    sf::Texture _texture;
    sf::Sprite _sprite;
    std::vector<sf::Uint8> _pixels;
    void rebuildPixels(const std::vector<std::vector<Tile>>& map);

public:
    SfmlDisplay(std::size_t width, std::size_t height);
    ~SfmlDisplay() = default;

    bool isOpen() const;
    bool pollEvents();
    void render(const std::vector<std::vector<Tile>>& map);
};

}

#endif
