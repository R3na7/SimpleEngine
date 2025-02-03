#ifndef MOUSE_HPP
#define MOUSE_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <array>


class Mouse {
public:
    Mouse(GLFWwindow * window);

    float getScrollX();
    float getScrollY();
    float getOffsetX();
    float getOffsetY();
    float getX() const;
    float getY() const;

    bool isButtonPressed(int button);
    bool isButtonPressedNow(int button);
    bool isButtonPressedRelease(int button);
    bool isScrolledX();
    bool isScrolledY();
    bool isOffset() const;
    bool isInWindow() const;
private:
    GLFWwindow * _window;

    double _xPos = 0.0f;
    double _yPos = 0.0f;

    float _xOffset = 0.0f;
    float _yOffset = 0.0f;

    float _sensitivity = 0.1f;

    int _scrollX = 0;
    int _scrollY = 0;

    std::array<bool, GLFW_MOUSE_BUTTON_LAST> _buttons {};
    std::array<bool, GLFW_MOUSE_BUTTON_LAST> _buttonsPress {};
    std::array<bool, GLFW_MOUSE_BUTTON_LAST> _buttonsRelease {};

    bool _inWindow = false;

    static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
    static void cursorEnterCallback   (GLFWwindow* window, int entered);
    static void mouseButtonCallback   (GLFWwindow* window, int button, int action, int mods);
    static void scrollCallback        (GLFWwindow* window, double xoffset, double yoffset);
};

#endif