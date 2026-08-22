
#include "main.hpp"

int main(int argc, char **argv) {
    raytracer::Argument arg(argc, argv);

    try {
        arg.Test();
    } catch (const IError& e) {
        if (e.code() == 84) {
            std::cerr << e;
            return 84;
        }
        std::cout << e.what();
        return 0;
    }
    try {
        raytracer::Core core(arg.GetSceneFile());
        core.Run();
    } catch (const IError& e) {
        if (e.code() == 84) {
            std::cerr << e;
            return 84;
        }
        std::cout << e;
        return 0;
    }

    return 0;
}