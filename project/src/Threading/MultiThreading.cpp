/*
** EPITECH PROJECT, 2025
** G-OOP-400-NCE-4-1-raytracer-8
** File description:
** MultiThreading.cpp - Thread orchestration only
*/

#include "MultiThread.hpp"

namespace raytracer {

static void placeolder(const std::vector<std::unique_ptr<IObject>>& objects,
    const std::vector<std::unique_ptr<ILight>>& lights,
    const render::Camera& camera,
    Tile& tile,
    size_t i,
    size_t y,
    const size_t pixelWidth) {
    (void)objects;
    (void)lights;
    (void)camera;
    (void)i;
    (void)y;
    (void)pixelWidth;

    tile.SetColor({255,255,0});
}

bool MultiThread::isEnd(void) {
    for (auto& f : thread)
        if (f.wait_for(std::chrono::seconds(0)) != std::future_status::ready)
            return 0;
    return 1;
}

int MultiThread::Compute(
    const std::vector<std::unique_ptr<IObject>>& objects,
    const std::vector<std::unique_ptr<ILight>>& lights,
    const render::Camera& camera,
    std::vector<std::vector<Tile>>& map,
    std::size_t start,
    std::size_t max,
    std::size_t& CalculingRow)
{
    for (auto& t: thread)
        t.get();
    thread.clear();

    std::cout << "Computing line " << start << " to " << max << "." << std::endl;

    const std::size_t pixelWidth = map[0].size();

    for (std::size_t y = start; y < max; y++) {
        thread.push_back(std::async(std::launch::async, [&objects, &lights, &camera, &map, y, &CalculingRow, pixelWidth]() {
            for (std::size_t i = 0; i < map[y].size(); ++i) {
                Tile& tile = map[y][i];
                placeolder(objects, lights, camera, tile, i, y, pixelWidth);
            }
            CalculingRow++;
        }));
    }
    return 1;
}

}