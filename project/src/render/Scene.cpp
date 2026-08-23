#include "Scene.hpp"

namespace raytracer {

GPUScene::GPUScene() {
    glGenBuffers(1, &_objectsSSBO);
    glGenBuffers(1, &_materialsSSBO);
}

GPUScene::~GPUScene() {
    if (_objectsSSBO)
        glDeleteBuffers(1, &_objectsSSBO);
    if (_materialsSSBO)
        glDeleteBuffers(1, &_materialsSSBO);
}

uint32_t GPUScene::addMaterial(const GPUMaterial &material) {
    _materials.push_back(material);
    _change = true;
    return static_cast<uint32_t>(_materials.size() - 1);
}

uint32_t GPUScene::addSphere(float x, float y, float z, float radius, uint32_t materialIndex) {
    _objects.emplace_back(Vec4f{x, y, z, radius}, GPUShapeType::Sphere, materialIndex);
    _change = true;
    return static_cast<uint32_t>(_objects.size() - 1);
}

void GPUScene::clear() {
    _objects.clear();
    _materials.clear();
    _change = true;
}

void GPUScene::upload() {
    if (!_change)
        return;

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, _objectsSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER,
        static_cast<GLsizeiptr>(_objects.size() * sizeof(GPUObject)),
        _objects.empty() ? nullptr : _objects.data(),
        GL_DYNAMIC_DRAW);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, _materialsSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER,
        static_cast<GLsizeiptr>(_materials.size() * sizeof(GPUMaterial)),
        _materials.empty() ? nullptr : _materials.data(),
        GL_DYNAMIC_DRAW);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    _change = false;
}

void GPUScene::bind(GLuint objectsBinding, GLuint materialsBinding) const {
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, objectsBinding, _objectsSSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, materialsBinding, _materialsSSBO);
}

}
