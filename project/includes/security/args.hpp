
#ifndef ARGS_HPP
    #define ARGS_HPP
    #include <string.h>
    #include <filesystem>
    #include <array>
    #include <sstream>
    #include <system_error>
    #include <exception>

    #include "Error.hpp"
    #include "Warning.hpp"

namespace raytracer {

class Argument {
    int argc;
    char **argv;
    std::string _sceneFile = "";

    void TestFile(std::string);

public:
    Argument(int argc, char **argv): argc(argc), argv(argv) {};
    ~Argument() = default;

    void Test();
    std::string GetSceneFile() {return _sceneFile;};
};

}

#endif
