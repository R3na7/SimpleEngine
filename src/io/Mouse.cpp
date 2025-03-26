#include "Mouse.hpp"

#include "InputObjects.hpp"

Mouse::Mouse(GLFWwindow * window)
: _window(window) {
    glfwSetCursorPosCallback  (_window, cursorPositionCallback);
    glfwSetMouseButtonCallback(_window, mouseButtonCallback);
    glfwSetScrollCallback     (_window, scrollCallback);
    glfwSetCursorEnterCallback(_window, cursorEnterCallback);
}

float Mouse::getX()       const {     return _xPos;     }
float Mouse::getY()       const {     return _yPos;     }
float Mouse::getScrollX() {
    float scrollX = _scrollX;
    _scrollX = 0.0f;
    
    return scrollX;
}
float Mouse::getScrollY() {
    float scrollY = _scrollY;
    _scrollY = 0.0f;
    return scrollY;
}
float Mouse::getOffsetX() {
    float xOffset = _xOffset;
    _xOffset = 0.0f;

    return xOffset;
}
float Mouse::getOffsetY() {
    float yOffset = _yOffset;
    _yOffset = 0.0f;

    return yOffset; 
}
GLFWwindow * Mouse::getLinkedWindow() {
    return _window;
}


void Mouse::cursorPositionCallback(GLFWwindow* window, double xPos, double yPos) {
    Mouse * mouse = static_cast<InputObjects *>(glfwGetWindowUserPointer(window))->_mouse;

    mouse->_xOffset = (xPos - mouse->_xPos) * mouse->_sensitivity;
    mouse->_yOffset = (yPos - mouse->_yPos) * mouse->_sensitivity;

    mouse->_xPos = xPos;
    mouse->_yPos = yPos;
}

void Mouse::cursorEnterCallback(GLFWwindow* window, int entered) {
    Mouse * mouse = static_cast<InputObjects *>(glfwGetWindowUserPointer(window))->_mouse;
    mouse->_inWindow = entered;
}

void Mouse::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    Mouse * mouse = static_cast<InputObjects *>(glfwGetWindowUserPointer(window))->_mouse;

    if (mouse) {
        if (action == GLFW_PRESS) {
            if (!mouse->_buttons[button]) {
                mouse->_buttonsPress[button] = true;
            }
            mouse->_buttons[button] = true;
        } else if (action == GLFW_RELEASE) {
            mouse->_buttons[button] = false;
            mouse->_buttonsRelease[button] = true;
        }
    }

}

void Mouse::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    Mouse * mouse = static_cast<InputObjects *>(glfwGetWindowUserPointer(window))->_mouse;

    mouse->_scrollX = xoffset;
    mouse->_scrollY = yoffset;
}

bool Mouse::isInWindow() const          {   return _inWindow;           }
bool Mouse::isButtonPressed(int button) {   return _buttons[button];    }
bool Mouse::isScrolledX()               {   return _scrollX != 0;       }
bool Mouse::isScrolledY()               {   return _scrollY != 0;       }
bool Mouse::isOffset() const            {   return _xOffset != 0.0f || _yOffset != 0.0f; }
bool Mouse::isButtonPressedNow(int button) {   
    bool pressed = _buttonsPress[button];
    _buttonsPress[button] = false;
    return pressed;
}
bool Mouse::isButtonPressedRelease(int button) {   
    bool release = _buttonsRelease[button];
    _buttonsRelease[button] = false;
    return release;
}

