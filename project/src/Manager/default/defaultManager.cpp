
#include "DefaultManager.hpp"

namespace raytracer {

void DefaultManager::Update(Scene& scene, const std::array<int, 3> BgCollor) {
    _state = WORKING;
    if (CalculatedRow >= scene._screen.size()) {
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
        int end = CalculatedRow + nb > scene._screen.size() ? scene._screen.size() : CalculatedRow + nb;
        _multiThread.Compute(scene, BgCollor, CalculatedRow, end, CalculingRow);
        CalculatedRow = end;
        _Printer.PrintLine(std::to_string((CalculingRow * 100) / scene._screen.size()) + "% Calculated.");
    } else {
        _Printer.ClearNbLine(1);
        _Printer.PrintLine(std::to_string((CalculingRow * 100) / scene._screen.size()) + "% Calculated.");
    }
    if (CalculingRow >= scene._screen.size() - 1) {
        _state = FINISH;
        return;
    }
}

}