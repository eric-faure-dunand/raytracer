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

static void draw_panels(Renderer &renderer, bool DoUpdate) {
    ImGui::Begin("Viewport");
    ImVec2 avail = ImGui::GetContentRegionAvail();
    int w = static_cast<int>(avail.x);
    int h = static_cast<int>(avail.y);
    if (w > 0 && h > 0) {
        if (DoUpdate) {
            std::chrono::steady_clock::time_point time = std::chrono::steady_clock::now();
            std::cout << Color::BLUE << "New frame Draw : " << Color::RESET << std::chrono::duration_cast<std::chrono::milliseconds>(time.time_since_epoch()).count() << std::endl;

            renderer.resize(w, h);
            renderer.render();
        }
        ImGui::Image(static_cast<ImTextureID>(renderer.texture()), avail, ImVec2(0, 1), ImVec2(1, 0));
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
    ImGui::TextDisabled("Aucun objet selectionne");
    ImGui::TextUnformatted("Fond de la scene (placeholder)");
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
    draw_panels(*renderer, _update);
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
    (void)fps;
    Camera cam = renderer->_scene._cam;
    std::array<float, 3> MoovVector = {0, 0, 0};

    if (ImGui::IsKeyDown(ImGuiKey_UpArrow))
        MoovVector += {cam.rotation[0] * MAXSPEED, cam.rotation[1] * MAXSPEED, cam.rotation[2] * MAXSPEED};
    if (ImGui::IsKeyDown(ImGuiKey_DownArrow))
        MoovVector -= {cam.rotation[0] * MAXSPEED, cam.rotation[1] * MAXSPEED, cam.rotation[2] * MAXSPEED};
    if (ImGui::IsKeyDown(ImGuiKey_RightArrow))
        MoovVector += cam.right;
    if (ImGui::IsKeyDown(ImGuiKey_LeftArrow))
        MoovVector -= cam.right;
    if (ImGui::IsKeyDown(ImGuiKey_Space))
        MoovVector[1] += MAXSPEED;
    if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
        MoovVector[1] -= MAXSPEED;

    if (MoovVector[0] != 0.0f || MoovVector[1] != 0.0f || MoovVector[2] != 0.0f)
        _update = true;
    std::cout << " MoovVector : [" << MoovVector[0] << ", " << MoovVector[1] << ", " << MoovVector[2] << "]" << std::endl;
    renderer->_scene._cam.position += MoovVector;
}

}
