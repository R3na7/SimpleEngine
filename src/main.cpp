
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Renderer.hpp"
#include "ObjectController.hpp"
#include "Utilities/ResourceManager.hpp"

int main() {
    Time::init();
    ResourceManager::init();
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    Window window(800, 600);
    window.setBackgroundColor(0.2f, 0.2f, 0.2f, 1.0f);
    
    Renderer renderer(window);
    
    Keyboard keyboard(window.getGLFWWindow());
    Mouse mouse(window.getGLFWWindow());
    InputObjects controllers(&keyboard, &mouse);

    World world;
    
    auto camera = world.add(Camera());

    auto green_cube = world.add({{Mesh::getCube({0.1f, 0.9f, 0.1f, 1.0f})}, "green"});

    auto blue_cube = world.add({{Mesh::getCube({0.1f, 0.1f, 0.9f, 1.0f})}, "blue"});
    blue_cube->scale({0.1f, 0.1f, 0.1f});

    auto spotLight = world.add(SpotLight());

    auto backpack = world.add(ResourceManager::loadModel("assets/models/back/Survival_BackPack_2.fbx"));
    backpack->ApplyTextureDiffuse("assets/models/backpack/diffuse.jpg");
    backpack->ApplyTextureSpecular("assets/models/backpack/specular.jpg");
    
    
    //world["green"]->translate_to_point({2.5f, -10.0f, 10.0f});
    //world["blue"]->translate_to_point({-2.5f, -10.0f, 10.0f});
    //world["blue"]->scale({0.1f, 0.1f, 0.1f});
    
    ObjectController camera_controller(world.getCurrentCamera(), &controllers);
    ObjectController spotLight_controller(spotLight, &controllers);
    ObjectController backpack_controller(backpack, &controllers);

    world.removeMesh(green_cube->getName());
    
    glfwSetInputMode(window.getGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    while (Window::havesWindow()) {
        
        if (mouse.isButtonPressed(GLFW_MOUSE_BUTTON_1)) {
            spotLight_controller.handleKeyboardInput();
            spotLight_controller.handleMouseInput();
            blue_cube->translate_to_point(spotLight->position());
        } else if (mouse.isButtonPressed(GLFW_MOUSE_BUTTON_2)) {
            backpack_controller.handleKeyboardInput();
            backpack_controller.handleMouseInput();
        } else {
            camera_controller.handleMouseInput();
            camera_controller.handleKeyboardInput();
        }
        


        renderer.render(world);
        window.setTitile(std::to_string(renderer.getFPS()).c_str());


        Time::update();
    }
    glfwTerminate();
}