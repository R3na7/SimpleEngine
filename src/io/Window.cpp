#ifndef WINDOW_CPP
#define WINDOW_CPP

#include "Window.hpp"
#include <thread>

int Window::_windowCount = 0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glfwMakeContextCurrent(window);
    glViewport(0, 0, width, height);
}

Window::Window(int width, int height, const char * title, bool isFullscreen)
: _width(width), _height(height), _title(title ? title : ""), _isFullscreen(isFullscreen) {

    if (_isFullscreen) {
        _window = glfwCreateWindow(_width, _height, _title.c_str(), glfwGetPrimaryMonitor(), NULL);
    } else {
        _window = glfwCreateWindow(_width, _height, _title.c_str(), NULL, NULL);
    }

    if (!_window) {
        std::cerr << "Failed to create GLFW window\n";
        return;
    }

    glfwMakeContextCurrent(_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        glfwDestroyWindow(_window);
        _window = nullptr;
        return;
    }
    glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);

    glEnable(GL_DEPTH_TEST);

    _isOpen = true;
    ++_windowCount;
}

Window::Window(int width, int height, const char * title, const Window & share, bool isFullscreen) 
: _width(width), _height(height), _title(title ? title : ""), _isFullscreen(isFullscreen) {

    if (_isFullscreen) {
        _window = glfwCreateWindow(_width, _height, _title.c_str(), glfwGetPrimaryMonitor(), share._window);
    } else {
        _window = glfwCreateWindow(_width, _height, _title.c_str(), NULL, share._window);
    }
    if (!_window) {
        std::cerr << "Failed to create GLFW window\n";
        return;
    }

    glfwMakeContextCurrent(_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        glfwDestroyWindow(_window);
        _window = nullptr;
        return;
    }

    glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);
    glEnable(GL_DEPTH_TEST);

    _isOpen = true;
    ++_windowCount;
}

Window::Window(const Window & window)
: Window(window._width, window._height, window._title.c_str(), window._isFullscreen) {
    _backgroundColor = window._backgroundColor;
}

bool Window::shouldClose() const {
    if (!_window) return true;
    if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) return true;

    // if (glfwGetKey(_window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    //     GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    //     const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    //     glfwSetWindowMonitor(_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);;
    // }
    return glfwWindowShouldClose(_window);
}


void Window::close() {
    if (!_isOpen) return;

    _isOpen = false;
    --_windowCount;
    if (_window) {
        glfwSetWindowShouldClose(_window, GLFW_TRUE);
    }
}

void Window::swapBuffers() {
    if (_window) {
        glfwSwapBuffers(_window);
    }
}

void Window::makeContext() {
    if (_window) {
        glfwMakeContextCurrent(_window);
    }
}

bool Window::havesWindow() {
    return _windowCount > 0;
}



GLFWwindow * Window::getGLFWWindow()   const  {    return _window;    }
std::string Window::getTitle()         const  {    return _title;     }
int Window::getWidht()                 const  {    return _width;     }
int Window::getHeight()                const  {    return _height;    }
glm::vec4 Window::getBackgroundColor() const  {    return glm::vec4(_backgroundColor[0], _backgroundColor[1], _backgroundColor[2], _backgroundColor[3]);   }
bool Window::getContext() const               {    return _window && glfwGetCurrentContext() == _window;   }

void Window::setWidht(int width)              {    _width = width;    }
void Window::setHeight(int height)            {    _height = height;  }
void Window::setTitile(const char * title)    {   
    _title = title ? title : "";
    if (_window) {
        glfwSetWindowTitle(_window, _title.c_str());
    }
}
void Window::setBackgroundColor (const glm::vec4 & backgroundColor) {
    _backgroundColor = backgroundColor;
}
void Window::setBackgroundColor (float r, float g, float b, float a) {
    _backgroundColor[0] = r;
    _backgroundColor[1] = g;
    _backgroundColor[2] = b;
    _backgroundColor[3] = a;
};



bool Window::isFullscreen() const  {   return _isFullscreen; }
bool Window::isOpen()       const  {   return _isOpen;       }



Window::~Window() {
    if (_isOpen) {
        --_windowCount;
    }
    if (_window) {
        glfwDestroyWindow(_window);
    }
    _window = nullptr;
    _isOpen = false;
}

#endif
