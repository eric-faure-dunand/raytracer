
#ifndef DISPLAY_HPP
    #define DISPLAY_HPP

struct GLFWwindow;

namespace raytracer {

// Fenetre + contexte OpenGL 4.3 + Dear ImGui (docking).
// Remplace l'ancien SfmlDisplay. Possede par le Core, pilote la boucle
// editeur. Le rendu du raytracer (texture GPU) viendra s'afficher dans le
// panneau Viewport.
class Display {
    GLFWwindow *_window = nullptr;

public:
    Display(int width, int height, const char *title);
    ~Display();

    Display(const Display&) = delete;
    Display& operator=(const Display&) = delete;

    bool isOpen() const;

    // Un cycle de frame : begin (events + dockspace), draw (panneaux), end (rendu + swap).
    void beginFrame();
    void drawEditor();
    void endFrame();
};

}

#endif
