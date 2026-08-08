#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <array>

class Keyboard {
public:
    Keyboard(GLFWwindow * window);
    
    bool isPressed(int key) const;

    bool isPressedNow(int key);

    bool isReleasedNow(int key);

    GLFWwindow * getLinkedWindow();

private:
    GLFWwindow * _window;
    std::array<bool, GLFW_KEY_LAST + 1> _keys = {};
    std::array<bool, GLFW_KEY_LAST + 1> _keysPressed = {};
    std::array<bool, GLFW_KEY_LAST + 1> _keysReleased = {};

    static void keyboardCallback(GLFWwindow * window, int key, int scancode, int action, int mods);
    
};

#endif
