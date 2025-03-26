#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include "string"

#include "../Mesh.hpp"

class Window final {
public:
    Window(int width, int height, const char * title = "", bool isFullscreen = false);

    Window(int width, int height, const char * title, const Window & share, bool isFullscreen = false);

    Window(const Window & window);

    bool shouldClose() const;

    void close();

    void swapBuffers();

    void makeContext();

    bool static havesWindow();

    GLFWwindow * getGLFWWindow()      const;
    std::string  getTitle()           const;
    int          getWidht()           const;
    int          getHeight()          const;
    glm::vec4    getBackgroundColor() const;
    bool         getContext()         const;

    void setWidht           (int width);
    void setTitile          (const char * title);
    void setHeight          (int height);
    void setBackgroundColor (float r, float g = 1.0f, float b = 1.0f, float a = 1.0f);
    void setBackgroundColor (const glm::vec4 & backgroundColor);
    

    bool isFullscreen () const;
    bool isOpen       () const;

    ~Window();
private:
    int _width;
    int _height;

    std::string _title;

    GLFWwindow *_window;
    
    glm::vec4 _backgroundColor {0.0f, 0.0f, 0.0f, 1.0f};

    bool _isFullscreen;
    bool _isOpen;

    static int _windowCount;
};

#endif