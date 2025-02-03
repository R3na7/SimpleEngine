#ifndef TIME_HPP
#define TIME_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Time{
public:
    static void init();
    static float DeltaTime();
    static float LastTime();
    static void update();

    ~Time();
private:
    Time() = default;

    static Time * _instance;

    float _lastTime;
    float _deltaTime;
};

#endif