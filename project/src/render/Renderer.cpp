#include "Renderer.hpp"

namespace raytracer {

static const char *kComputeSource = R"(#version 430
layout(local_size_x = 8, local_size_y = 8) in;
layout(rgba8, binding = 0) uniform image2D img;

void main() {
    ivec2 pixel = ivec2(gl_GlobalInvocationID.xy);
    ivec2 size = imageSize(img);

    if (pixel.x >= size.x || pixel.y >= size.y)
        return;

    imageStore(img, pixel, vec4(0.0, 0.0, 1.0, 1.0));
}
)";

Renderer::Renderer() {
    createProgram();
}

Renderer::~Renderer() {
    if (_texture)
        glDeleteTextures(1, &_texture);
    if (_program)
        glDeleteProgram(_program);
}

void Renderer::createProgram() {
    // 1) Compiler le compute shader.
    GLuint shader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(shader, 1, &kComputeSource, nullptr);
    glCompileShader(shader);

    GLint ok = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[1024];
        glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
        glDeleteShader(shader);
        throw Error("Renderer: compute shader compilation failed:\n" + std::string(log));
    }

    // 2) Lier le programme.
    _program = glCreateProgram();
    glAttachShader(_program, shader);
    glLinkProgram(_program);
    glDeleteShader(shader); // le shader compile est copie dans le programme

    glGetProgramiv(_program, GL_LINK_STATUS, &ok);
    if (!ok) {
        char log[1024];
        glGetProgramInfoLog(_program, sizeof(log), nullptr, log);
        throw Error("Renderer: program link failed:\n" + std::string(log));
    }
}

void Renderer::allocTexture(int w, int h) {
    // Une texture GL est a taille fixe : pour changer de taille on recree.
    if (_texture)
        glDeleteTextures(1, &_texture);

    glGenTextures(1, &_texture);
    glBindTexture(GL_TEXTURE_2D, _texture);
    // Stockage immuable RGBA 8 bits par canal, 1 niveau de mipmap.
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, w, h);
    // Filtrage lineaire pour l'affichage a l'ecran.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    _width = w;
    _height = h;
}

void Renderer::resize(int w, int h) {
    if (w <= 0 || h <= 0)
        return;
    if (w == _width && h == _height)
        return;
    allocTexture(w, h);
}

void Renderer::render() {
    if (!_texture)
        return;

    glUseProgram(_program);

    // On expose la texture au shader en tant qu'"image" en ecriture, unite 0
    // (le meme binding = 0 que dans le GLSL).
    glBindImageTexture(0, _texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);

    // Nombre de blocs 8x8 necessaires pour couvrir toute l'image (arrondi haut).
    GLuint groupsX = (static_cast<GLuint>(_width) + 7) / 8;
    GLuint groupsY = (static_cast<GLuint>(_height) + 7) / 8;
    glDispatchCompute(groupsX, groupsY, 1);

    // On attend que les ecritures dans la texture soient visibles avant que
    // quelqu'un (ImGui) ne la lise pour l'afficher.
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

}
