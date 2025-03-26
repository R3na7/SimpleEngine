#include "ObjectController.hpp"

ObjectController::ObjectController(std::shared_ptr<Object> object, InputObjects * controller)
: _object(object), _controller(controller) {
    
}

void ObjectController::handleKeyboardInput() {

    if (_controller->_keyboard->isPressed(GLFW_KEY_W)) {
        auto lookAt = std::move(_object->lookAt());

        float x = glm::dot(lookAt, glm::vec3{1.0f, 0.0f, 0.0f});
        float z = glm::dot(lookAt, glm::vec3{0.0f, 0.0f, 1.0f});

        _object->translate(
            glm::normalize(glm::vec3{x, 0.0f, z}) * 3.0f * Time::DeltaTime()
        );
    }
    if (_controller->_keyboard->isPressed(GLFW_KEY_S)) {
        auto lookAt = _object->lookAt();
        
        float x = glm::dot(lookAt, glm::vec3{1.0f, 0.0f, 0.0f});
        float z = glm::dot(lookAt, glm::vec3{0.0f, 0.0f, 1.0f});
        
        _object->translate(
            glm::normalize(-glm::vec3{x, 0.0f, z}) * 3.0f * Time::DeltaTime()
        );
    }
    if (_controller->_keyboard->isPressed(GLFW_KEY_A)) {
        auto left = _object->left();
        float x = glm::dot(left, glm::vec3{1.0f, 0.0f, 0.0f});
        float z = glm::dot(left, glm::vec3{0.0f, 0.0f, 1.0f});
        
        _object->translate(
            glm::normalize(glm::vec3{x, 0.0f, z}) * 3.0f * Time::DeltaTime()
        );
    }
    if (_controller->_keyboard->isPressed(GLFW_KEY_D)) {
        auto left = _object->left();
        float x = glm::dot(left, glm::vec3{1.0f, 0.0f, 0.0f});
        float z = glm::dot(left, glm::vec3{0.0f, 0.0f, 1.0f});
        
        _object->translate(
            glm::normalize(-glm::vec3{x, 0.0f, z}) * 3.0f * Time::DeltaTime()
        );
    }
    if (_controller->_keyboard->isPressed(GLFW_KEY_LEFT_SHIFT)) {
        _object->translate(
            glm::vec3{0.0f, -1.0f, 0.0f} * 3.0f * Time::DeltaTime()
        );
    }
    if (_controller->_keyboard->isPressed(GLFW_KEY_SPACE)) {
        _object->translate(
            glm::vec3{0.0f, 1.0f, 0.0f} * 3.0f * Time::DeltaTime()
        );
    }
    if (_controller->_keyboard->isPressed(GLFW_KEY_E)) {
        _object->Yaw(100.0f * Time::DeltaTime());
    }
    if (_controller->_keyboard->isPressed(GLFW_KEY_Q)) {
        _object->Yaw(-100.0f * Time::DeltaTime());
    }


}

void ObjectController::handleMouseInput() {

    if (_controller->_mouse->isOffset()) {
        _object->Yaw(_controller->_mouse->getOffsetX());
        _object->Pitch(-_controller->_mouse->getOffsetY());
    }

}
