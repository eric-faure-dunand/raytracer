#include "UI.hpp"

namespace raytracer {

static void glfw_error_callback(int error, const char *description) {
    std::cerr << "GLFW error " << error << ": " << description << std::endl;
}

static void draw_dockspace() {
    const ImGuiViewport *vp = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(vp->WorkPos);
    ImGui::SetNextWindowSize(vp->WorkSize);
    ImGui::SetNextWindowViewport(vp->ID);

    ImGuiWindowFlags host_flags =
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
        ImGuiWindowFlags_NoDocking;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockHost", nullptr, host_flags);
    ImGui::PopStyleVar(3);

    ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

    static bool layout_built = false;
    if (!layout_built) {
        layout_built = true;
        ImGui::DockBuilderRemoveNode(dockspace_id);
        ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockspace_id, vp->WorkSize);

        ImGuiID center = dockspace_id;
        ImGuiID right  = ImGui::DockBuilderSplitNode(center, ImGuiDir_Right, 0.25f, nullptr, &center);
        ImGuiID left   = ImGui::DockBuilderSplitNode(center, ImGuiDir_Left,  0.22f, nullptr, &center);
        ImGuiID center_bottom = ImGui::DockBuilderSplitNode(center, ImGuiDir_Down, 0.28f, nullptr, &center);

        ImGui::DockBuilderDockWindow("Viewport",   center);
        ImGui::DockBuilderDockWindow("Hierarchy",  center_bottom);
        ImGui::DockBuilderDockWindow("Add Object", left);
        ImGui::DockBuilderDockWindow("Inspector",  right);
        ImGui::DockBuilderFinish(dockspace_id);
    }

    ImGui::End();
}

void UI::draw_panels(Renderer &renderer) {
    ImGui::Begin("Viewport");
    ImVec2 avail = ImGui::GetContentRegionAvail();
    int w = static_cast<int>(avail.x);
    int h = static_cast<int>(avail.y);
    if (w > 0 && h > 0) {
        if (_update) {
            std::chrono::steady_clock::time_point time = std::chrono::steady_clock::now();
            std::cout << Color::BLUE << "New frame Draw : " << Color::RESET << std::chrono::duration_cast<std::chrono::milliseconds>(time.time_since_epoch()).count() << std::endl;

            renderer.resize(w, h);
            renderer.render();
        }
        ImGui::Image(static_cast<ImTextureID>(renderer.texture()), avail, ImVec2(0, 1), ImVec2(1, 0));
    }
    if (_showPos) {
        std::ostringstream stream;
        stream << renderer._scene._cam.position;
        std::string text(stream.str());
        ImVec2 text_size = ImGui::CalcTextSize(text.c_str());
        float padding = 4.0f;

        ImGui::SetCursorPos(ImVec2(8, 40));
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 0.6f));
        ImGui::BeginChild("TextBg", ImVec2(text_size.x + padding * 2, text_size.y + padding * 2), false, ImGuiWindowFlags_NoScrollbar);
        ImGui::SetCursorPos(ImVec2(padding, padding));
        ImGui::Text("%s", text.c_str());
        ImGui::EndChild();
        ImGui::PopStyleColor();
    }
    ImGui::End();

    ImGui::Begin("Hierarchy");
    ImGui::TextDisabled("Objets de la scene (vide)");
    ImGui::End();

    ImGui::Begin("Add Object");
    ImGui::Button("Sphere");
    ImGui::Button("Plane");
    ImGui::Button("Triangle");
    ImGui::End();

    ImGui::Begin("Inspector");
    ImGui::Checkbox("Show Coordinates", &_showPos);
    ImGui::SliderFloat("Rotation speed", &RotationSpeed, 0.1f, 5.0f, "%.2f rad/s");
    ImGui::SliderFloat("Moovement speed", &MoovSpeed, 0.1f, 5.0f, "%.2f rad/s");
    ImGui::End();
}

UI::UI(int width, int height, const char *title) {
    glfwSetErrorCallback(glfw_error_callback);
#ifdef __linux__
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
#endif
    if (!glfwInit())
        throw Error("UI: glfwInit failed");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    _window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!_window) {
        glfwTerminate();
        throw Error("UI: window creation failed");
    }
    glfwMakeContextCurrent(_window);
    glfwSwapInterval(1);

    if (!gladLoadGL(glfwGetProcAddress)) {
        glfwDestroyWindow(_window);
        glfwTerminate();
        throw Error("UI: failed to load OpenGL via glad");
    }
    std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(_window, true);
    ImGui_ImplOpenGL3_Init("#version 430");
}

UI::~UI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    if (_window)
        glfwDestroyWindow(_window);
    glfwTerminate();
}

bool UI::isOpen() const{
    return _window && !glfwWindowShouldClose(_window);
}

void UI::beginFrame() {
    glfwPollEvents();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    draw_dockspace();
    ImGui::Begin("Viewport");
    if (_last_size != ImGui::GetContentRegionAvail())
        _update = true;
    _last_size = ImGui::GetContentRegionAvail();
    ImGui::End();
}

void UI::drawEditor(std::unique_ptr<Renderer>& renderer) {
    draw_panels(*renderer);
    _update = false;
}

void UI::endFrame() {
    ImGui::Render();
    int w = 0;
    int h = 0;
    glfwGetFramebufferSize(_window, &w, &h);
    glViewport(0, 0, w, h);
    glClearColor(0.90f, 0.90f, 0.92f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(_window);
}

void UI::event(std::unique_ptr<Renderer>& renderer, uint8_t& fps) {
    Camera &cam = renderer->_scene._cam;
    Vector3 moveVector = {0, 0, 0};

    if (ImGui::IsKeyDown(ImGuiKey_Z))
        moveVector += {cam.rotation.x * MoovSpeed, cam.rotation.y * MoovSpeed, cam.rotation.z * MoovSpeed};
    if (ImGui::IsKeyDown(ImGuiKey_S))
        moveVector -= {cam.rotation.x * MoovSpeed, cam.rotation.y * MoovSpeed, cam.rotation.z * MoovSpeed};
    if (ImGui::IsKeyDown(ImGuiKey_D))
        moveVector += {cam.right.x * MoovSpeed, cam.right.y * MoovSpeed, cam.right.z * MoovSpeed};
    if (ImGui::IsKeyDown(ImGuiKey_Q))
        moveVector -= {cam.right.x * MoovSpeed, cam.right.y * MoovSpeed, cam.right.z * MoovSpeed};
    if (ImGui::IsKeyDown(ImGuiKey_Space))
        moveVector.y += MoovSpeed;
    if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
        moveVector.y -= MoovSpeed;

    const float dt = (fps > 0) ? 1.0f / static_cast<float>(fps) : 0.0f;

    float deltaYaw = 0.0f;
    float deltaPitch = 0.0f;
    if (ImGui::IsKeyDown(ImGuiKey_RightArrow))
        deltaYaw += RotationSpeed * dt;
    if (ImGui::IsKeyDown(ImGuiKey_LeftArrow))
        deltaYaw -= RotationSpeed * dt;
    if (ImGui::IsKeyDown(ImGuiKey_UpArrow))
        deltaPitch += RotationSpeed * dt;
    if (ImGui::IsKeyDown(ImGuiKey_DownArrow))
        deltaPitch -= RotationSpeed * dt;

    Vector3 oldRotation = cam.rotation;
    if (deltaYaw != 0.0f || deltaPitch != 0.0f)
        cam.ApplyYawPitch(deltaYaw, deltaPitch);
    if (moveVector != Vector3(0.0f, 0.0f, 0.0f) || cam.rotation != oldRotation)
        _update = true;
    cam.position += moveVector;
}

}
