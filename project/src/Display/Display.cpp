#include "Display.hpp"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_internal.h" // DockBuilder*
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "Error.hpp"

#include <cstdio>

namespace raytracer {

static void glfw_error_callback(int error, const char *description)
{
    std::fprintf(stderr, "GLFW error %d: %s\n", error, description);
}

// Fenetre hote plein ecran qui contient le DockSpace, + layout par defaut
// construit une seule fois.
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

        ImGui::DockBuilderDockWindow("Viewport",   center);        // haut-centre
        ImGui::DockBuilderDockWindow("Hierarchy",  center_bottom); // sous le viewport
        ImGui::DockBuilderDockWindow("Add Object", left);          // colonne gauche
        ImGui::DockBuilderDockWindow("Inspector",  right);         // colonne droite
        ImGui::DockBuilderFinish(dockspace_id);
    }

    ImGui::End();
}

static void draw_panels()
{
    // Viewport : accueillera l'image rendue par le compute shader.
    ImGui::Begin("Viewport");
    ImGui::TextUnformatted("Simulation (placeholder)");
    ImGui::TextDisabled("La sortie du raytracer viendra ici.");
    ImGui::End();

    // Hierarchy : liste des objets presents dans la scene.
    ImGui::Begin("Hierarchy");
    ImGui::TextDisabled("Objets de la scene (vide)");
    ImGui::End();

    // Add Object : liste des objets ajoutables.
    ImGui::Begin("Add Object");
    ImGui::Button("Sphere");
    ImGui::Button("Plane");
    ImGui::Button("Triangle");
    ImGui::End();

    // Inspector : parametres de l'objet selectionne, sinon du fond de scene.
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
    glClearColor(0.10f, 0.10f, 0.12f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(_window);
}

}
