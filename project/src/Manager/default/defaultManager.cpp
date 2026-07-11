/*
** EPITECH PROJECT, 2025
** G-OOP-400-NCE-4-1-raytracer-8
** File description:
** defaultManager.cpp
*/

#include "DefaultManager.hpp"

namespace raytracer {

void DefaultManager::Update(const std::vector<std::unique_ptr<IObject>>& objects, const std::vector<std::unique_ptr<ILight>>& lights, const render::Camera& camera, std::vector<std::vector<Tile>>& map) {
    _state = WORKING;
    if (CalculatedRow >= map.size()) {
        _state = FINISH;
        return;
    }
    if (_multiThread.isEnd()) {
        if (CalculatedRow != 0) {
            if (CalculingRow == CalculatedRow)
                _Printer.ClearNbLine(2);
            else
                _Printer.ClearNbLine(1);
        }
        int nb = _multiThread.GetTreadNumber();
        int end = CalculatedRow + nb > map.size() ? map.size() : CalculatedRow + nb;
        _multiThread.Compute(objects, lights, camera, map, CalculatedRow, end, CalculingRow);
        CalculatedRow = end;
        _Printer.PrintLine(std::to_string((CalculingRow * 100) / map.size()) + "% Calculated.");
    } else {
        _Printer.ClearNbLine(1);
        _Printer.PrintLine(std::to_string((CalculingRow * 100) / map.size()) + "% Calculated.");
    }
    if (CalculingRow >= map.size() - 1) {
        _state = FINISH;
        return;
    }
}

}