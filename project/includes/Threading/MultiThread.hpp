/*
** EPITECH PROJECT, 2025
** G-OOP-400-NCE-4-1-raytracer-8
** File description:
** MultiThread.hpp
*/

#ifndef MULTITHREAD_HPP
    #define MULTITHREAD_HPP
    #include <vector>
    #include <future>
    #include <chrono>
    #include <iostream>

    #include "Tile.hpp"
    #include "IObject.hpp"
    #include "ILight.hpp"
    #include "Camera.hpp"

namespace raytracer {

class MultiThread {
    std::vector<std::future<void>> thread = {};
    const int numThreads = std::thread::hardware_concurrency();

public:
    MultiThread() {std::cout << numThreads << " thread Created."<< std::endl;};
    ~MultiThread() = default;

    bool isEnd(void);
    int Compute(const std::vector<std::unique_ptr<IObject>>& objects, const std::vector<std::unique_ptr<ILight>>& lights, const render::Camera& camera, std::vector<std::vector<Tile>>& line, std::size_t y, std::size_t max, std::size_t& CalculingRow);

    int GetTreadNumber(void) {return numThreads;};
};

}

#endif