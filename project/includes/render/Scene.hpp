
#ifndef GPUSCENE_HPP
    #define GPUSCENE_HPP
    #include <cstdint>
    #include <vector>
    #include <glad/gl.h>

    #include "Types.hpp"
    #include "Camera.hpp"

namespace raytracer {

class GPUScene {
    GLuint _objectsSSBO = 0;
    GLuint _materialsSSBO = 0;

    bool _change = true;

public:
    std::vector<GPUObject> _objects;
    std::vector<GPUMaterial> _materials;
    Camera _cam;

    GPUScene();
    ~GPUScene();

    GPUScene(const GPUScene&) = delete;
    GPUScene& operator=(const GPUScene&) = delete;

    uint32_t addMaterial(const GPUMaterial &material);
    uint32_t addSphere(float x, float y, float z, float radius, uint32_t materialIndex);

    void clear();
    void upload();

    void bind(GLuint objectsBinding, GLuint materialsBinding) const;

    std::size_t objectCount() const { return _objects.size(); }
    std::size_t materialCount() const { return _materials.size(); }
};

}

#endif
