
#include "MultiThread.hpp"

namespace raytracer {

static void placeolder(const std::vector<std::unique_ptr<IObject>>& objects,
    const std::vector<std::unique_ptr<ILight>>& lights,
    const render::Camera& camera,
    Tile& tile,
    size_t i,
    size_t y,
    const size_t pixelWidth,
    const std::array<int, 3> BgColor) {
    (void)objects;
    (void)lights;
    (void)camera;
    (void)i;
    (void)y;
    (void)pixelWidth;

    tile.SetColor(BgColor);
}

bool MultiThread::isEnd(void) {
    for (auto& f : thread)
        if (f.wait_for(std::chrono::seconds(0)) != std::future_status::ready)
            return 0;
    return 1;
}

int MultiThread::Compute(
    Scene& scene,
    const std::vector<std::unique_ptr<IObject>>& objects,
    const std::vector<std::unique_ptr<ILight>>& lights,
    const std::array<int, 3> BgColor,
    std::size_t start,
    std::size_t max,
    std::size_t& CalculingRow)
{
    for (auto& t: thread)
        t.get();
    thread.clear();

    std::cout << "Computing line " << start << " to " << max << "." << std::endl;

    const std::size_t pixelWidth = scene._screen[0].size();

    for (std::size_t y = start; y < max; y++) {
        thread.push_back(std::async(std::launch::async, [&scene, &objects, &lights, y, &CalculingRow, pixelWidth, BgColor]() {
            for (std::size_t i = 0; i < scene._screen[y].size(); ++i) {
                Tile& tile = scene._screen[y][i];
                placeolder(objects, lights, scene._camera, tile, i, y, pixelWidth, BgColor);
            }
            CalculingRow++;
        }));
    }
    return 1;
}

}