
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

    Window window(800, 600, "window - red - container");
    window.setBackgroundColor(0.2f, 0.2f, 0.2f, 1.0f);

    Shader shader("assets/shaders/vertex.vs", "assets/shaders/fragment.fs");

    Renderer renderer(&window, &shader);

    Keyboard keyboard(window.getGLFWWindow());
    Mouse mouse(window.getGLFWWindow());
    InputObjects controllers(window.getGLFWWindow(), &keyboard, &mouse);

    Camera camera;

    Mesh cube = Mesh::getCube("cube", {0.1f, 0.9f, 0.1f, 1.0f});

    World world("world", {&camera});

    Model green_cube({std::make_shared<Mesh>(cube)}, "green");
    cube.setColor({0.1f, 0.1f, 0.9f, 1.0f});
    Model blue_cube({std::make_shared<Mesh>(cube)}, "blue");
    
    Model backpack = ResourceManager::loadModel("assets/models/back/Survival_BackPack_2.fbx");
    backpack.ApplyTextureDiffuse("assets/models/backpack/diffuse.jpg");
    backpack.ApplyTextureSpecular("assets/models/backpack/specular.jpg");

    world.addModel(green_cube);
    world.addModel(blue_cube);
    world.addModel(backpack);

    DirectionLight dirlight;
    world.addDirectionLight(&dirlight);

    world["green"]->translate_to_point({2.5f, -10.0f, 10.0f});
    world["blue"]->translate_to_point({-2.5f, -10.0f, 10.0f});

    ObjectController camera_controller(world[camera.getName()], &controllers);


    glfwSetInputMode(window.getGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    while (Window::havesWindow()) {   
        camera_controller.mouseInputs();
        camera_controller.keyboardInputs();
        





        renderer.render(world);
        window.setTitile(std::to_string(renderer.getFPS()).c_str());

        glfwSwapInterval(1);
        glfwPollEvents();
        Time::update();
    }
    glfwTerminate();
}