
#ifndef AREADER_HPP
    #define AREADER_HPP

    #include "IReader.hpp"
    #include "libld.hpp"

namespace raytracer {

class AReader : public IReader {
protected:
    ld ldloader;
    std::string _sceneFile;
    std::vector<std::string> NoOpenFile;
public:
    AReader(std::string NameFile): _sceneFile(NameFile) {};
    ~AReader() {};

    void SetUp() = 0;
    std::pair<std::size_t, std::size_t> GetCameraResolution() = 0;
    std::array<float, 3> GetCameraPosition() = 0;
    std::array<float, 3> GetCameraRotation() = 0;
    double GetCameraFieldOfView() = 0;

    std::vector<std::unique_ptr<IObject>> GetObjects() = 0;
    std::vector<std::unique_ptr<ILight>> GetLights() = 0;

    std::vector<std::string> GetNoOpenFile() {return NoOpenFile;};
};

}

#endif