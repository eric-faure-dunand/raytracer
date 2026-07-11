/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** Tile.hpp
*/

#ifndef TILE_HPP
    #define TILE_HPP
    #include <array>

namespace raytracer {

typedef enum {
    NOTCOMPUTE = -1,
    COMPUTING = 0,
    COMPUTED = 1,
} TileState;

class Tile {
    std::array<int, 3> _color;
    TileState _state = NOTCOMPUTE;

public:
    Tile(std::array<int, 3> color): _color(color) {};
    ~Tile() = default;

    void SetRed(int r) {_color[0] = r;};
    void SetGreen(int g) {_color[1] = g;};
    void SetBlue(int b) {_color[2] = b;};
    void SetColor(std::array<int, 3> color) {_color = color;};
    void SetState(TileState state) {_state = state;};

    int GetRed(void) const {return _color[0];};
    int GetGreen(void) const {return _color[1];};
    int GetBlue(void) const {return _color[2];};
    std::array<int, 3> GetColor(void) const {return _color;};
    TileState GetState(void) const {return _state;};
};

}

#endif
