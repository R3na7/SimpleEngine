#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <memory>

#include "io/window.hpp"
#include "Mesh.hpp"
#include "Camera.hpp"
#include "World.hpp"

class Renderer {
public:
    Renderer(Window * window, Shader * shader);

    void render(const World & world); 
private:
    std::unique_ptr<Window> _window;
    std::unique_ptr<Shader> _shader;
};

#endif