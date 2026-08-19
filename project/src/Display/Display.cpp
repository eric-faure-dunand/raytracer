#include "Display.hpp"

namespace raytracer {

static void glfw_error_callback(int error, const char *description)
{
    std::fprintf(stderr, "GLFW error %d: %s\n", error, description);
}

static void draw_dockspace()
{
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

static void draw_panels()
{
    ImGui::Begin("Viewport");
    ImVec2 avail = ImGui::GetContentRegionAvail();
    std::string format = "available size = " + std::to_string(avail.x) + " " + std::to_string(avail.y);
    ImGui::TextUnformatted(format.c_str());
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

Display::Display(int width, int height, const char *title)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        throw Error("Display: glfwInit failed");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    _window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!_window) {
        glfwTerminate();
        throw Error("Display: window creation failed");
    }
    glfwMakeContextCurrent(_window);
    glfwSwapInterval(1); // vsync

    if (!gladLoadGL(glfwGetProcAddress)) {
        glfwDestroyWindow(_window);
        glfwTerminate();
        throw Error("Display: failed to load OpenGL via glad");
    }
    std::printf("OpenGL %s\n", glGetString(GL_VERSION));

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(_window, true);
    ImGui_ImplOpenGL3_Init("#version 430");
}

Display::~Display()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    if (_window)
        glfwDestroyWindow(_window);
    glfwTerminate();
}

bool Display::isOpen() const
{
    return _window && !glfwWindowShouldClose(_window);
}

void Display::beginFrame()
{
    glfwPollEvents();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    draw_dockspace();
}

void Display::drawEditor()
{
    draw_panels();
}

void Display::endFrame()
{
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

}
