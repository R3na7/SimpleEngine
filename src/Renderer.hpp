#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <memory>

#include "io/Window.hpp"
#include "Mesh.hpp"
#include "Camera.hpp"
#include "World.hpp"

class Renderer {
public:
    Renderer(Window & window);

    int getFPS() const;

    void clearScreen() const;

    void setupCamera(const World & world) const;

    void render(const World & world); 

    void updateFps();
private:
    float deltaTime = 0;
    int frameCount = 0;
    int fps = 0;
    
    Window * _window;
    std::unique_ptr<Shader> _shaderModels;
    

    void renderModels(const World & world) const;
    void renderLights(const World & world) const;
    void renderLine(const glm::vec3 & firstPoint, const glm::vec3 & secondPoint) const;
    void renderMesh(const Mesh & mesh) const;
};

#endif
