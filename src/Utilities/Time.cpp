#include "Time.hpp"

Time * Time::_instance = nullptr;

void Time::init() {
    static Time instance;
    _instance = &instance;
    _instance->_lastTime = static_cast<float>(glfwGetTime());
    _instance->_deltaTime = 0.0f;
}

float Time::DeltaTime() {   return _instance ? _instance->_deltaTime : 0.0f;    }
float Time::LastTime () {   return _instance ? _instance->_lastTime : 0.0f;     }

void Time::update() {
    if (!_instance) return;
    const float currentTime = static_cast<float>(glfwGetTime());
    _instance->_deltaTime = currentTime - _instance->_lastTime;
    _instance->_lastTime = currentTime;
}
