
#ifndef DISPLAY_HPP
    #define DISPLAY_HPP
    #include <cstdio>
    #include <memory>
    #include <glad/gl.h>
    #include <GLFW/glfw3.h>
    #include <string>
    #include <iostream>
    #include <chrono>
    #include <cmath>
    #include <sstream>
    #include <optional>

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

class LittelRender {
    Renderer render;
public:
    LittelRender() : render(false) {};
    ~LittelRender() = default;

    ImTextureID GetObjectTrombi(const GPUObject Object, const GPUMaterial Material, const ImVec2 ImageSize);
};

class UI {
    GLFWwindow *_window = nullptr;

    ImVec2 _last_size = {0, 0};

    bool _update = true;

    bool _showPos = true;
    bool _showAngle = false;

    float MoovSpeed = 1.5f;
    float RotationSpeed = 1.2f;

    std::optional<LittelRender> TrombiRender;
    std::vector<ImTextureID> ObjectTrombi;
    int selected_index = -1;

    void draw_panels(Renderer &renderer);

public:
    UI(int width, int height, const char *title);
    ~UI();

    UI(const UI&) = delete;
    UI& operator=(const UI&) = delete;

    bool isOpen() const;

    void beginFrame();
    void drawEditor(std::unique_ptr<Renderer>& renderer);
    void endFrame();
    void event(std::unique_ptr<Renderer>& renderer, uint8_t& _fps);
};

inline bool operator!=(const ImVec2& first, const ImVec2& second) {
    if (first.x == second.x && first.y == second.y)
        return false;
    return true;
}

};

#endif
