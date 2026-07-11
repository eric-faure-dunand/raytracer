#include "SfmlDisplay.hpp"

namespace raytracer {

SfmlDisplay::SfmlDisplay(std::size_t width, std::size_t height)
    : _window(sf::VideoMode(static_cast<unsigned int>(width), static_cast<unsigned int>(height)), "Raytracer")
    , _pixels(width * height * 4, 0) {
    if (width == 0 || height == 0)
        throw std::runtime_error("Cannot create display with empty dimensions");
    if (!_texture.create(static_cast<unsigned int>(width), static_cast<unsigned int>(height)))
        throw std::runtime_error("Cannot create SFML texture");
    _sprite.setTexture(_texture, true);
    _window.setFramerateLimit(60);
}

bool SfmlDisplay::isOpen() const {
    return _window.isOpen();
}

bool SfmlDisplay::pollEvents() {
    sf::Event event{};

    while (_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            _window.close();
            return false;
        }
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            _window.close();
            return false;
        }
    }
    return _window.isOpen();
}

void SfmlDisplay::rebuildPixels(const std::vector<std::vector<Tile>>& map) {
    _pixels.clear();
    _pixels.reserve(map.size() * map[0].size() * 4);

    for (const auto& row : map)
        for (const auto& pixel : row) {
            _pixels.push_back(static_cast<sf::Uint8>(pixel.GetRed()));
            _pixels.push_back(static_cast<sf::Uint8>(pixel.GetGreen()));
            _pixels.push_back(static_cast<sf::Uint8>(pixel.GetBlue()));
            _pixels.push_back(255);
        }
}

void SfmlDisplay::render(const std::vector<std::vector<Tile>>& map) {
    if (!_window.isOpen() || map.empty() || map[0].empty())
        return;

    rebuildPixels(map);
    _texture.update(_pixels.data());

    _window.clear();
    _window.draw(_sprite);
    _window.display();
}

}
