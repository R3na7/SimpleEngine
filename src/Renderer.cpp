#include "Renderer.hpp"
#include "Utilities/Time.hpp"
#include <algorithm>

namespace {
constexpr int LightArraySize = 10;
constexpr int MaterialTextureArraySize = 5;
}

Renderer::Renderer(Window & window)
: 
_window(&window), 
_shaderModels(std::make_unique<Shader>("assets/shaders/vertexModel.vs", "assets/shaders/fragmentModel.fs"))
{

}

int Renderer::getFPS() const {  return fps; }

void Renderer::clearScreen() const {
    glClearColor(_window->getBackgroundColor().r, _window->getBackgroundColor().g, _window->getBackgroundColor().b, _window->getBackgroundColor().a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::setupCamera(const World & world) const {
    _shaderModels->uniformMatrix4("projection", world.getCurrentCamera()->getProjectionMatrix());
    _shaderModels->uniformMatrix4("view"      , world.getCurrentCamera()->getViewMatrix());
    _shaderModels->uniform3f("viewPos", world.getCurrentCamera()->position());
}

void Renderer::updateFps() {
    deltaTime += Time::DeltaTime();
    ++frameCount;

    if (deltaTime >= 1.0f) {
        fps = frameCount;

        deltaTime = 0.0f;
        frameCount = 0;
    }
}

void Renderer::renderLights(const World & world) const {
    int index = 0;
    
    _shaderModels->uniform1i("pointLightCount", static_cast<int>(std::min(
        world.getPointLights().size(), static_cast<std::size_t>(LightArraySize))));
    for (const auto & pointLight : world.getPointLights()) {
        if (index >= LightArraySize) break;
        _shaderModels->uniform3f("pointLights[" + std::to_string(index) + "]._position",  pointLight->position());

        _shaderModels->uniform1f("pointLights[" + std::to_string(index) + "]._constant",  pointLight->getConstant());
        _shaderModels->uniform1f("pointLights[" + std::to_string(index) + "]._linear",    pointLight->getLinear());
        _shaderModels->uniform1f("pointLights[" + std::to_string(index) + "]._quadratic", pointLight->getQuadratic());

        _shaderModels->uniform3f("pointLights[" + std::to_string(index) + "]._ambient",   pointLight->getAmbient());
        _shaderModels->uniform3f("pointLights[" + std::to_string(index) + "]._diffuse",   pointLight->getDiffuse());
        _shaderModels->uniform3f("pointLights[" + std::to_string(index) + "]._specular",  pointLight->getSpecular());
        index += 1;
    }
    index = 0;

    _shaderModels->uniform1i("spotLightCount", static_cast<int>(std::min(
        world.getSpotLights().size(), static_cast<std::size_t>(LightArraySize))));
    for (const auto & spotLight : world.getSpotLights()) {
        if (index >= LightArraySize) break;
        _shaderModels->uniform3f("spotLights[" + std::to_string(index) + "]._position",    spotLight->position());
        _shaderModels->uniform3f("spotLights[" + std::to_string(index) + "]._direction",   spotLight->lookAt());
        _shaderModels->uniform1f("spotLights[" + std::to_string(index) + "]._cutOff",      glm::cos(glm::radians(spotLight->getCutOff())));
        _shaderModels->uniform1f("spotLights[" + std::to_string(index) + "]._outerCutOff", glm::cos(glm::radians(spotLight->getOuterCutOff())));
        _shaderModels->uniform1f("spotLights[" + std::to_string(index) + "]._constant",    spotLight->getConstant());
        _shaderModels->uniform1f("spotLights[" + std::to_string(index) + "]._linear",      spotLight->getLinear());
        _shaderModels->uniform1f("spotLights[" + std::to_string(index) + "]._quadratic",   spotLight->getQuadratic());

        _shaderModels->uniform3f("spotLights[" + std::to_string(index) + "]._ambient",     spotLight->getAmbient());
        _shaderModels->uniform3f("spotLights[" + std::to_string(index) + "]._diffuse",     spotLight->getDiffuse());
        _shaderModels->uniform3f("spotLights[" + std::to_string(index) + "]._specular",    spotLight->getSpecular());
        index += 1;
    }
    index = 0;

    _shaderModels->uniform1i("directionLightCount", static_cast<int>(std::min(
        world.getDirectionLights().size(), static_cast<std::size_t>(LightArraySize))));
    for (const auto & directionLight : world.getDirectionLights()) {
        if (index >= LightArraySize) break;
        _shaderModels->uniform3f("directionLights[" + std::to_string(index) + "]._direction",  directionLight->getDirection());

        _shaderModels->uniform3f("directionLights[" + std::to_string(index) + "]._ambient",   directionLight->getAmbient());
        _shaderModels->uniform3f("directionLights[" + std::to_string(index) + "]._diffuse",   directionLight->getDiffuse());
        _shaderModels->uniform3f("directionLights[" + std::to_string(index) + "]._specular",  directionLight->getSpecular());
        index += 1;
    }
}

void Renderer::renderMesh(const Mesh & mesh) const {
    mesh.bindVertexArray();

    _shaderModels->uniformMatrix4("modelMesh", mesh.getModel());
    _shaderModels->uniform4f("ourColor", mesh.getColor());
    _shaderModels->uniform1f("ourShininess", mesh.getShininess());
    
    int index = 0;

    int id = 0;
    for (const auto & texture : mesh.getTexturesDiffuse()) {
        if (index >= MaterialTextureArraySize) break;
        if (!texture || !texture->isLoaded()) {
            continue;
        }
        texture->bindTextureDiffuse(*_shaderModels, id, index);
        id += 1;
        index += 1;
    }
    _shaderModels->uniform1i("materialDiffuseCount", index);
    index = 0;
    for (const auto & texture : mesh.getTexturesSpecular()) {
        if (index >= MaterialTextureArraySize) break;
        if (!texture || !texture->isLoaded()) {
            continue;
        }
        texture->bindTextureSpecular(*_shaderModels, id, index);
        id += 1;
        index += 1;
    }
    _shaderModels->uniform1i("materialSpecularCount", index);
    index = 0;
    for (const auto & texture : mesh.getTexturesEmbient()) {
        if (index >= MaterialTextureArraySize) break;
        if (!texture || !texture->isLoaded()) {
            continue;
        }
        texture->bindTextureAmbient(*_shaderModels, id, index);
        id += 1;
        index += 1;
    }
    _shaderModels->uniform1i("materialEmbientCount", index);
    
    glDrawElements(GL_TRIANGLES  , mesh.getIndicesCount(), GL_UNSIGNED_INT, 0);    
}

void Renderer::renderLine(const glm::vec3 & firstPoint, const glm::vec3 & secondPoint) const {

}

void Renderer::renderModels(const World & world) const {
    for (auto & model : world.getModels()) {

        _shaderModels->uniformMatrix4("modelModel", model->getModel());

        for (auto & mesh : model->getMeshes()) {
            renderMesh(mesh);
        }
    }
}

void Renderer::render(const World & world) {
    if (!_window->isOpen()) return;
    if (_window->shouldClose()) {
        _window->close();
        return;
    }

    _window->makeContext();
    _shaderModels->use();

    clearScreen();
    setupCamera(world);

    renderLights(world);
    renderModels(world);


    updateFps();

    _window->swapBuffers();
    glfwSwapInterval(1);
    glfwPollEvents();
}
