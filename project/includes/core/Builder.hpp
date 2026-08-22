
#ifndef BUILDER_HPP
    #define BUILDER_HPP
    #include <cstdint>
    #include <memory>
    #include <string>

    #include "args.hpp"
    #include "CfgReader.hpp"

namespace raytracer {

class ReaderBuilder {
    std::string _sceneFile;

public:
    ReaderBuilder& SetSceneFile(const std::string& sceneFile);

    std::unique_ptr<IReader> BuildReader() const;
};

}

#endif
