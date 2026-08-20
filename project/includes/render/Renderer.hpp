
#ifndef RENDERER_HPP
    #define RENDERER_HPP
    #include <glad/gl.h>

namespace raytracer {

// Rendu GPU de la simulation.
//
// Principe : le GPU ne "dessine" pas directement dans la fenetre ici. Il
// ecrit une image dans une TEXTURE (un tableau 2D de pixels en memoire GPU),
// et c'est ImGui qui affiche ensuite cette texture dans le panneau Viewport.
//
// Aujourd'hui le compute shader remplit tout de bleu. Demain, ce meme compute
// shader lancera un rayon par pixel : c'est le SEUL endroit qui changera pour
// passer du placeholder au vrai raytracer. Le reste (texture, affichage) ne
// bouge plus.
class Renderer {
    GLuint _texture = 0;  // l'image produite par le GPU
    GLuint _program = 0;  // le compute shader compile
    int _width = 0;
    int _height = 0;

    void createProgram();
    void allocTexture(int w, int h);

public:
    Renderer();
    ~Renderer();

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    // (Re)alloue la texture uniquement si la taille demandee a change.
    void resize(int w, int h);

    // Lance le compute shader : il ecrit dans la texture.
    void render();

    GLuint texture() const { return _texture; }
    int width() const { return _width; }
    int height() const { return _height; }
};

}

#endif
