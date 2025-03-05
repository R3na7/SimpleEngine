
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

    Window first_window(800, 600, "first_window - red - container");
    first_window.setBackgroundColor(0.2f, 0.2f, 0.2f, 1.0f);

    Shader first_shader("assets/shaders/vertex.vs", "assets/shaders/fragment.fs");

    Renderer first_render(&first_window, &first_shader);

    Keyboard keyboard(first_window.getGLFWWindow());
    Mouse mouse(first_window.getGLFWWindow());
    InputObjects controllers(first_window.getGLFWWindow(), &keyboard, &mouse);

    Camera camera;

    Texture magma("assets/textures/magma.jpg");
    Texture container("assets/textures/container2.png");
    Texture container_specular("assets/textures/container2_specular.png");

    Texture light("assets/textures/brightness.png");

    Mesh cube = Mesh::getCube("cube", {0.1f, 0.9f, 0.1f, 1.0f});

    World world("world", {&camera});
    //Model model = ResourceManager::loadModel("assets/models/backpack/backpack.obj");
    Model model = ResourceManager::loadModel("assets/models/back/Survival_BackPack_2.fbx");
    model.ApplyTextureDiffuse("assets/models/backpack/diffuse.jpg");
    model.ApplyTextureSpecular("assets/models/backpack/specular.jpg");

    Model green_cube({std::make_shared<Mesh>(cube)}, "green");
    cube.setColor({0.1f, 0.1f, 0.9f, 1.0f});
    Model blue_cube({std::make_shared<Mesh>(cube)}, "blue");
    
    green_cube.scale({0.1f, 0.1f, 0.1f});
    blue_cube.scale({0.1f, 0.1f, 0.1f});

    world.addModel(model);
    world.addModel(green_cube);
    world.addModel(blue_cube);

    PointLight light_green({0.1f, 0.2f, 0.1f}, {0.1f, 0.9f, 0.1f}, {0.1f, 0.9f, 0.1f});
    PointLight light_blue({0.1f, 0.1f, 0.2f}, {0.1f, 0.1f, 0.9f}, {0.1f, 0.1f, 0.9f});

    DirectionLight dirlight;

    light_blue.setname("lighte_blue");
    world.addPointLight(&light_green);
    world.addPointLight(&light_blue);
    world.addDirectionLight(&dirlight);

    ObjectController camera_controller(world[camera.getName()], &controllers);
    ObjectController light_green_contriller(world[light_green.getName()], &controllers);
    ObjectController light_blue_controller(world[light_blue.getName()], &controllers);
    ObjectController model_controller(world[model.getName()], &controllers);

    int fps = 0;
    float temp = 0;

    glfwSetInputMode(first_window.getGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    while (Window::havesWindow())
    {   
        Time::update();

        temp += Time::DeltaTime();
        ++fps;

        if (temp >= 1.0f) {
            temp = 0.0f;
            first_window.setTitile(std::to_string(fps).c_str());
            fps = 0;
        }

        if (keyboard.isPressed(GLFW_KEY_K)) {
            light_green_contriller.keyboardInputs();
        } 
        else if (keyboard.isPressed(GLFW_KEY_L)) {
            light_blue_controller.keyboardInputs();
        }
        else if (keyboard.isPressed(GLFW_KEY_J)) {
            model_controller.keyboardInputs();
            model_controller.mouseInputs();
        } 
        else {
            camera_controller.mouseInputs();
            camera_controller.keyboardInputs();
        }

        world[green_cube.getName()]->translate_to_point(world[light_green.getName()]->position());
        world[blue_cube.getName()]->translate_to_point(world[light_blue.getName()]->position());

        
        first_render.render(world);

        glfwSwapInterval(1);
        glfwPollEvents();
    }
    glfwTerminate();
}