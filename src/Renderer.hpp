#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <memory>

#include "io/Window.hpp"
#include "Mesh.hpp"
#include "Camera.hpp"
#include "World.hpp"

class Renderer {
public:
    Renderer(Window * window, Shader * shader);

    int getFPS() const;

    void render(const World & world); 
private:
    float deltaTime = 0;
    int frameCount = 0;
    int fps = 0;
    
    std::unique_ptr<Window> _window;
    std::unique_ptr<Shader> _shader;
};

#endif
