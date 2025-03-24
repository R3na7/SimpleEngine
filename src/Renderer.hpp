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

    void clearScreen() const;

    void setupCamera(const World & world) const;

    void render(const World & world); 

    void updateFps();
private:
    float deltaTime = 0;
    int frameCount = 0;
    int fps = 0;
    
    std::unique_ptr<Window> _window;
    std::unique_ptr<Shader> _shader;

    void renderModels(const World & world) const;
    void renderLights(const World & world) const;
    void renderMesh(const std::shared_ptr<Mesh> mesh) const;
};

#endif
