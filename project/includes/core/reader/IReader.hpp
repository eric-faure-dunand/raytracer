
#ifndef IREADER_HPP
    #define IREADER_HPP
    #include <string>
    #include <utility>
    #include <cstddef>
    #include <vector>
    #include <memory>

    #include "MyVector.hpp"
    #include "Error.hpp"

namespace raytracer {

    class IReader {
    public:
        virtual ~IReader() = default;
        virtual void SetUp() = 0;

        virtual std::pair<std::size_t, std::size_t> GetCameraResolution() = 0;
        virtual Vector3 GetCameraPosition() = 0;
        virtual Vector3 GetCameraRotation() = 0;
        virtual double GetCameraFieldOfView() = 0;

        //virtual std::vector<std::unique_ptr<IObject>> GetObjects() = 0;
        //virtual std::vector<std::unique_ptr<ILight>> GetLights() = 0;

        virtual std::vector<std::string> GetNoOpenFile() = 0;
    };

}

#endif