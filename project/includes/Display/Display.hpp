
#ifndef DISPLAY_HPP
    #define DISPLAY_HPP
    #include <cstdio>
    #include <memory>
    #include <glad/gl.h>
    #include <GLFW/glfw3.h>

    #include "imgui.h"
    #include "imgui_internal.h"
    #include "backends/imgui_impl_glfw.h"
    #include "backends/imgui_impl_opengl3.h"

    #include "Error.hpp"

struct GLFWwindow;

namespace raytracer {

class Renderer;

class Display {
    GLFWwindow *_window = nullptr;
    std::unique_ptr<Renderer> _renderer;

public:
    Display(int width, int height, const char *title);
    ~Display();

    Display(const Display&) = delete;
    Display& operator=(const Display&) = delete;

    bool isOpen() const;

    void beginFrame();
    void drawEditor();
    void endFrame();
};

}

#endif
