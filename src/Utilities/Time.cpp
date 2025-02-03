#include "Time.hpp"

Time * Time::_instance = nullptr;

void Time::init() {
    delete _instance;
    _instance = new Time;
}

float Time::DeltaTime() {   return _instance->_deltaTime;    }
float Time::LastTime () {   return _instance->_lastTime;     }

void Time::update() {
    if (!_instance) return;

    _instance->_deltaTime = glfwGetTime() - _instance->_lastTime;
    _instance->_lastTime = glfwGetTime();
}

Time::~Time() {
    delete _instance;
}