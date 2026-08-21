
#ifndef DISPLAY_HPP
    #define DISPLAY_HPP
    #include <cstdio>
    #include <memory>
    #include <glad/gl.h>
    #include <GLFW/glfw3.h>
    #include <string>
    #include <iostream>
    #include <chrono>

    #include "imgui.h"
    #include "imgui_internal.h"
    #include "backends/imgui_impl_glfw.h"
    #include "backends/imgui_impl_opengl3.h"
    #include "Renderer.hpp"

    #include "Error.hpp"
    #include "printer.hpp"

struct GLFWwindow;

namespace raytracer {

class Renderer;

class Display {
    GLFWwindow *_window = nullptr;
    std::unique_ptr<Renderer> _renderer;

    ImVec2 _last_size = {0, 0};

    bool _update = true;

public:
    Display(int width, int height, const char *title);
    ~Display();

    Display(const Display&) = delete;
    Display& operator=(const Display&) = delete;

    bool isOpen() const;

    void beginFrame();
    void drawEditor(const Camera cam);
    void endFrame();
};

inline bool operator!=(const ImVec2& first, const ImVec2& second) {
    if (first.x == second.x && first.y == second.y)
        return false;
    return true;
}

}

#endif
