#ifndef WINDOW_CPP
#define WINDOW_CPP

#include "window.hpp"
#include <thread>

int Window::_windowCount = 0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glfwMakeContextCurrent(window);
    glViewport(0, 0, width, height);
}

Window::Window(int width, int height, const char * title, bool isFullscreen)
: _width(width), _height(height), _title(title), _isFullscreen(isFullscreen), _isOpen(true) {   

    if (_isFullscreen) {
        _window = glfwCreateWindow(_width, _height, _title.c_str(), glfwGetPrimaryMonitor(), NULL);
    } else {
        _window = glfwCreateWindow(_width, _height, _title.c_str(), NULL, NULL);
    }

    glfwMakeContextCurrent(_window);
    
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);

    glEnable(GL_DEPTH_TEST);

    _isOpen = true;
    ++_windowCount;
}

Window::Window(int width, int height, const char * title, const Window & share, bool isFullscreen) 
: _width(width), _height(height), _title(title), _isFullscreen(isFullscreen), _isOpen(true) {
    

    if (_isFullscreen) {
        _window = glfwCreateWindow(_width, _height, _title.c_str(), glfwGetPrimaryMonitor(), share._window);
    } else {
        _window = glfwCreateWindow(_width, _height, _title.c_str(), NULL, share._window);
    }
    std::cout << "shared\n";
    glfwMakeContextCurrent(share._window);

    glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);


    

    _isOpen = true;
    ++_windowCount;
}

bool Window::shouldClose() const {
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

    glfwDestroyWindow(_window);
    _isOpen = false;
    --_windowCount;
}

void Window::swapBuffers() {
    glfwSwapBuffers(_window);
}

void Window::makeContext() {
    glfwMakeContextCurrent(_window);
}

bool Window::havesWindow() {
    return _windowCount;
}



GLFWwindow * Window::getGLFWWindow()   const  {    return _window;    }
std::string Window::getTitle()         const  {    return _title;     }
int Window::getWidht()                 const  {    return _width;     }
int Window::getHeight()                const  {    return _height;    }
glm::vec4 Window::getBackgroundColor() const  {    return glm::vec4(_backgroundColor[0], _backgroundColor[1], _backgroundColor[2], _backgroundColor[3]);   }

void Window::setWidht(int width)              {    _width = width;    }
void Window::setHeight(int height)            {    _height = height;  }
void Window::setTitile(const char * title)    {   
    _title = title;    
    glfwSetWindowTitle(_window, _title.c_str());
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
    glfwDestroyWindow(_window);
}

#endif