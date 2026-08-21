#include "Renderer.hpp"
#include "Shader.hpp"

namespace raytracer {

Renderer::Renderer() {
    createProgram();
    buildDemoScene();
}

Renderer::~Renderer() {
    if (_texture)
        glDeleteTextures(1, &_texture);
    if (_program)
        glDeleteProgram(_program);
}

void Renderer::createProgram() {
    _program = Shader::compileCompute("shaders/raytrace.comp");
}

void Renderer::buildDemoScene() {
    uint32_t red  = _scene.addMaterial(GPUMaterial({0.8f, 0.2f, 0.2f, 1.0f}, GPUEffect::Diffuse));
    uint32_t blue = _scene.addMaterial(GPUMaterial({0.2f, 0.4f, 0.9f, 1.0f}, GPUEffect::Diffuse));

    _scene.addSphere(0.0f, 0.0f, -3.0f, 1.0f, red);
    _scene.addSphere(1.5f, -0.3f, -4.0f, 0.7f, blue);
}

void Renderer::setCameraUniforms(const Camera cam) {
    glUniform3f(glGetUniformLocation(_program, "camPos"), cam.position[0] , cam.position[1], cam.position[2]);
    glUniform3f(glGetUniformLocation(_program, "camForward"), 0.0f, 0.0f, -1.0f);
    glUniform3f(glGetUniformLocation(_program, "camRight"),1.0f, 0.0f, 0.0f);
    glUniform3f(glGetUniformLocation(_program, "camUp"),0.0f, 1.0f, 0.0f);
    glUniform1f(glGetUniformLocation(_program, "tanHalfFov"), 0.5773503f);
}

void Renderer::allocTexture(int w, int h) {
    if (_texture)
        glDeleteTextures(1, &_texture);

    glGenTextures(1, &_texture);
    glBindTexture(GL_TEXTURE_2D, _texture);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, w, h);
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

void Renderer::render(const Camera cam) {
    if (!_texture)
        return;

    glUseProgram(_program);

    _scene.upload();
    _scene.bind(1, 2);
    setCameraUniforms(cam);

    glBindImageTexture(0, _texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);

    GLuint groupsX = (static_cast<GLuint>(_width) + 7) / 8;
    GLuint groupsY = (static_cast<GLuint>(_height) + 7) / 8;
    glDispatchCompute(groupsX, groupsY, 1);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);
}

}
