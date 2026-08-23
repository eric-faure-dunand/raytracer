
#ifndef GPUTYPES_HPP
    #define GPUTYPES_HPP
    #include <cstdint>

namespace raytracer {

struct Vec4f {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float w = 0.0f;
};

enum class GPUShapeType : uint32_t {
    Sphere = 0,
};

enum class GPUEffect : uint32_t {
    Diffuse  = 0,
    Mirror   = 1,
    Glass    = 2,
    Emissive = 3,
};

struct alignas(16) GPUObject {
    Vec4f    posRadius;
    uint32_t type = 0;
    uint32_t materialIndex = 0;
    uint32_t _pad0 = 0;
    uint32_t _pad1 = 0;

    GPUObject() = default;
    GPUObject(Vec4f pr, GPUShapeType shapeType, uint32_t material)
        : posRadius(pr), type(static_cast<uint32_t>(shapeType)), materialIndex(material) {}
};

struct alignas(16) GPUMaterial {
    Vec4f    albedo {1.0f, 1.0f, 1.0f, 1.0f};
    uint32_t effect = 0;
    uint32_t textureIndex = 0xFFFFFFFFu;
    float    roughness = 1.0f;
    uint32_t _pad0 = 0;

    GPUMaterial() = default;
    GPUMaterial(Vec4f baseAlbedo, GPUEffect materialEffect,
        uint32_t texture = 0xFFFFFFFFu, float materialRoughness = 1.0f)
        : albedo(baseAlbedo), effect(static_cast<uint32_t>(materialEffect)),
        textureIndex(texture), roughness(materialRoughness) {}
};

static_assert(sizeof(GPUObject) % 16 == 0, "GPUObject doit rester aligne std430 (multiple de 16)");
static_assert(sizeof(GPUMaterial) % 16 == 0, "GPUMaterial doit rester aligne std430 (multiple de 16)");

}

#endif
