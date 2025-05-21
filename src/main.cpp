
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

    auto green_cube = world.add(Model::getCube({0.1f, 0.9f, 0.1f, 1.0f}, 0.2f, "green"));
    auto blue_cube = world.add(Model::getCube({0.1f, 0.1f, 0.9f, 1.0f}, 0.2f, "blue"));

    auto greenPointLight = world.add(PointLight({0.0f, 0.1f, 0.0f}, {0.0f, 0.8f, 0.0f}, {0.0f, 0.8f, 0.0f}));
    auto bluePointLight = world.add(PointLight({0.0f, 0.0f, 0.1f}, {0.0f, 0.0f, 0.8f}, {0.0f, 0.0f, 0.8f}));

    //auto directionLight = world.add(DirectionLight());
    
    auto backpack = world.add(ResourceManager::loadModel("assets/models/back/Survival_BackPack_2.fbx"));
    backpack->ApplyTextureDiffuse("assets/models/back/1001_albedo.jpg");
    backpack->ApplyTextureSpecular("assets/models/back/1001_metallic.jpg");

    world["green"]->translate_to_point({2.5f, -10.0f, 10.0f});
    world["blue"]->translate_to_point({-2.5f, -10.0f, 10.0f});
    
    ObjectController camera_controller(world.getCurrentCamera(), &controllers);
    ObjectController greenCubeController(green_cube, &controllers);
    ObjectController blueCubeController(blue_cube, &controllers);
    
    glfwSetInputMode(window.getGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    while (Window::havesWindow()) {
        
        if (keyboard.isPressed(GLFW_KEY_J)) {
            greenCubeController.handleKeyboardInput();
            greenCubeController.handleMouseInput();
        } else if (keyboard.isPressed(GLFW_KEY_K)) {
            blueCubeController.handleKeyboardInput();
            blueCubeController.handleMouseInput();
        } else {
            camera_controller.handleMouseInput();
            camera_controller.handleKeyboardInput();
        }
        
        greenPointLight->translate_to_point(green_cube->position());
        bluePointLight->translate_to_point(blue_cube->position());

        renderer.render(world);
        window.setTitile(std::to_string(renderer.getFPS()).c_str());


        Time::update();
    }
    glfwTerminate();
}