#include "SfmlDisplay.hpp"

namespace raytracer {

SfmlDisplay::SfmlDisplay(std::size_t width, std::size_t height)
    : _window(sf::VideoMode(static_cast<unsigned int>(width), static_cast<unsigned int>(height)), "Raytracer")
    , _pixels(width * height * 4, 0) {
    if (width == 0 || height == 0)
        throw std::runtime_error("Cannot create display with empty dimensions");

    uint32_t sceen_x = width < 800 ? width : 800;
    uint32_t sreen_y = height < 600 ? height : 600;

    if (!_screen.create(static_cast<unsigned int>(sceen_x), static_cast<unsigned int>(sreen_y)))
            throw std::runtime_error("Cannot create SFML texture");
        _sprite.setTexture(_screen, true);
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
    _screen.update(_pixels.data());

    _window.clear();
    _window.draw(_sprite);
    _window.display();
}

}
