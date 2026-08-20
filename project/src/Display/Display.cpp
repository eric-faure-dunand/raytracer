#include "Display.hpp"
#include "Renderer.hpp"

#include <string>

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

static void draw_panels(Renderer &renderer)
{
    // Viewport : on rend la simulation a la taille exacte du panneau, puis on
    // affiche la texture produite par le GPU.
    ImGui::Begin("Viewport");
    ImVec2 avail = ImGui::GetContentRegionAvail();
    int w = static_cast<int>(avail.x);
    int h = static_cast<int>(avail.y);
    if (w > 0 && h > 0) {
        renderer.resize(w, h);
        renderer.render();
        // La texture GL a son origine en bas a gauche, ImGui dessine de haut
        // en bas : on retourne verticalement via uv0=(0,1) / uv1=(1,0).
        ImGui::Image(static_cast<ImTextureID>(renderer.texture()), avail,
                     ImVec2(0, 1), ImVec2(1, 0));
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

Display::Display(int width, int height, const char *title)
{
    glfwSetErrorCallback(glfw_error_callback);
#ifdef __linux__
    // Sous WSLg, le backend Wayland gere mal le maximize (surface GL qui ne
    // grandit pas d'un coup -> contour noir). X11 (XWayland) le gere bien.
    // Ignore sur Windows/macOS. A reevaluer pour un Linux Wayland natif.
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
#endif
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

    // Le contexte GL est pret : on peut creer le renderer (il compile son shader).
    _renderer = std::make_unique<Renderer>();
}

Display::~Display()
{
    // Detruire les objets GPU TANT QUE le contexte GL est encore vivant.
    // (Sinon ~Renderer ferait des appels GL apres glfwTerminate -> segfault.)
    _renderer.reset();

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
    draw_panels(*_renderer);
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
