
#ifndef DISPLAY_HPP
    #define DISPLAY_HPP

    #include <glad/gl.h>
    #include <GLFW/glfw3.h>

    #include "imgui.h"
    #include "imgui_internal.h" // DockBuilder*
    #include "backends/imgui_impl_glfw.h"
    #include "backends/imgui_impl_opengl3.h"

    #include "Error.hpp"

    #include <cstdio>

struct GLFWwindow;

namespace raytracer {

class Display {
    GLFWwindow *_window = nullptr;

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
