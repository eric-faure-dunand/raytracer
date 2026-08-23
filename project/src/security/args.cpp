
#include "args.hpp"

namespace raytracer {

void Argument::TestFile(std::string files) {
    std::error_code ec;

    if (!std::filesystem::exists(files, ec)) {
        if (ec)
            throw Error(files + " : " + ec.message() + ".");
        throw Error(files + " : File not found.");
    }
}

void Argument::Test() {
    if (argc == 1)
        return;
    if (argc > 2)
        throw Error("Too many argument, try ./raytracer --help.\n");
    if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
        throw Warning("USAGE:\n\t./raytracer <SCENE_FILE>"
        "\n\nSCENE_FILE:\n\tthe scene configuration"
        "\n\nOPTION:"
        "\n\t<SCENE_FILE> : lunch the raytracer with the file configuration."
        "\n\twithout file : lunch raytracer in default mode."
        "\n\n\t-h --help    : show this help."
        "\n");
    TestFile(std::string(argv[1]));
    _sceneFile = argv[1];
}


}

