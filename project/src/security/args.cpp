/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** args.cpp
*/

#include "args.hpp"

namespace raytracer {

std::size_t Argument::TestPort(std::string port) {
    std::size_t nb = 0;
    std::size_t idx = 0;
    try {
        nb = std::stoull(port, &idx);
    } catch (const std::exception&) {
        throw Error(port + " : Not a valid port.");
    }
    if (idx != port.size() || nb == 0 || nb > 65535)
        throw Error(port + " : Not a valid port.");
    return nb;
}

void Argument::TestIp(const std::string ip) {
    std::istringstream ss(ip);
    std::array<int, 4> octets;
    char dot;

    for (int i = 0; i < 4; ++i)
        if (!(ss >> octets[i]) || (octets[i] < 0 || octets[i] > 255) || (i < 3 && (!(ss >> dot) || dot != '.')))
            throw Error(ip + " is not a valid Ip.");
    if (!(ss.eof() || ss.peek() == EOF))
        throw Error(ip + " is not a valid Ip.");
}

void Argument::TestFile(std::string files) {
    std::error_code ec;

    if (!std::filesystem::exists(files, ec)) {
        if (ec)
            throw Error(files + " : " + ec.message() + ".");
        throw Error(files + " : File not found.");
    }
}

void Argument::Test() {
    if (argc < 2)
        throw Error("Too few argument.\n");
    if (argc > 4)
        throw Error("Too few argument.\n");
    if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
        throw Warning("USAGE:\n\t./raytracer [-option] <SCENE_FILE> [...]\n\nSCENE_FILE:\n\tscene configuration"
        "\n\nOPTION:\n\tnone, -d, --default + <SCENE_FILE> : lunch the raytracer in default mode."
        "\n\t-s, --server + <SCENE_FILE> <port> : lunch the raytracer in mode server on the port <port>."
        "\n\t-c, --client + <ip> <port> \t   : lunch the raytracer in client mode and connect it at the ip <ip> and on the port <port>."
        "\n");

    std::string type = argv[1];

    if ((type == "-d" || type == "--default") && argc == 3) {
        TestFile(argv[2]);
        _sceneFile = argv[2];
    }
    else if ((type == "-s" || type == "--server") && argc == 4) {
        _type = SERVER;
        TestFile(argv[2]);
        _sceneFile = argv[2];
        _port = TestPort(argv[3]);
    } else if ((type == "-c" || type == "--client") && argc == 4) {
        _type = CLIENT;
        TestIp(argv[2]);
        _ip = argv[2];
        _port = TestPort(argv[3]);
    } else if (argc == 2) {
        TestFile(std::string(argv[1]));
        _sceneFile = argv[1];
    } else
        throw Error("Invalid argument, try ./raytracer --help.");
}


}

