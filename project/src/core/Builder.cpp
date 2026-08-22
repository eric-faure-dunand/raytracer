
#include "Builder.hpp"

namespace raytracer {

ReaderBuilder& ReaderBuilder::SetSceneFile(const std::string& sceneFile) {
    _sceneFile = sceneFile;
    return *this;
}

std::unique_ptr<IReader> ReaderBuilder::BuildReader() const {
    if (_sceneFile.empty())
        return nullptr;

    auto pos = _sceneFile.rfind('.');
    if (pos == std::string::npos)
        return nullptr;

    std::string ext = _sceneFile.substr(pos);
    if (ext == ".cfg")
        return std::make_unique<CfgReader>(_sceneFile);
    if (ext == ".obj")
        return nullptr;

    return nullptr;
}

}
