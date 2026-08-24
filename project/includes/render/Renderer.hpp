
#ifndef RENDERER_HPP
    #define RENDERER_HPP
    #include <glad/gl.h>
    #include <string>

    #include "Shader.hpp"
    #include "Error.hpp"
    #include "Scene.hpp"
    #include "Camera.hpp"

namespace raytracer {

class Renderer {
    GLuint _texture = 0;
    GLuint _program = 0;
    int _width = 0;
    int _height = 0;

    void createProgram();
    void allocTexture(int w, int h);
    void buildDemoScene();
    void setCameraUniforms();

public:

    GPUScene _scene;

    Renderer();
    Renderer(bool demo);
    ~Renderer();

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    void resize(int w, int h);

    void render();

    GLuint texture() const { return _texture; }
    int width() const { return _width; }
    int height() const { return _height; }
};

}

#endif
