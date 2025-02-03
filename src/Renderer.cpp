#include "Renderer.hpp"


Renderer::Renderer(Window * window, Shader * shader)
: _window(window), _shader(shader) {}

void Renderer::render(const World & world) {
    if (!_window->isOpen()) return;
    if (_window->shouldClose()) _window->close();

    _window->makeContext();
    _shader->use();

    glClearColor(_window->getBackgroundColor().r, _window->getBackgroundColor().g, _window->getBackgroundColor().b, _window->getBackgroundColor().a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _shader->uniformMatrix4("projection", world.getCurrentCamera().getProjectionMatrix());
    _shader->uniformMatrix4("view"      , world.getCurrentCamera().getViewMatrix());
    _shader->uniform3f("viewPos", world.getCurrentCamera().position());
    for (auto & model : world.getModels()) {

        _shader->uniformMatrix4("modelModel", model->getModel());

        for (auto & mesh : model->getMeshes()) {

            mesh->bindVertexArray();

            _shader->uniformMatrix4("modelMesh", mesh->getModel());
            _shader->uniform4f("ourColor", mesh->getColor());
            _shader->uniform1f("ourShininess", mesh->getShininess());

            int index = 0;

            _shader->uniform1i("pointLightCount", world.getPointLights().size());
            for (const auto & pointLight : world.getPointLights()) {
                _shader->uniform3f("pointLights[" + std::to_string(index) + "]._position",  pointLight->position());

                _shader->uniform1f("pointLights[" + std::to_string(index) + "]._constant",  pointLight->getConstant());
                _shader->uniform1f("pointLights[" + std::to_string(index) + "]._linear",    pointLight->getLinear());
                _shader->uniform1f("pointLights[" + std::to_string(index) + "]._quadratic", pointLight->getQuadratic());

                _shader->uniform3f("pointLights[" + std::to_string(index) + "]._ambient",   pointLight->getAmbient());
                _shader->uniform3f("pointLights[" + std::to_string(index) + "]._diffuse",   pointLight->getDiffuse());
                _shader->uniform3f("pointLights[" + std::to_string(index) + "]._specular",  pointLight->getSpecular());
                index += 1;
            }
            index = 0;

            _shader->uniform1i("spotLightCount", world.getSpotLights().size());
            for (const auto & spotLight : world.getSpotLights()) {
                _shader->uniform3f("spotLights[" + std::to_string(index) + "]._position",    spotLight->position());
                _shader->uniform3f("spotLights[" + std::to_string(index) + "]._direction",   spotLight->lookAt());
                _shader->uniform1f("spotLights[" + std::to_string(index) + "]._cutOff",      glm::cos(glm::radians(spotLight->getCutOff())));
                _shader->uniform1f("spotLights[" + std::to_string(index) + "]._outerCutOff", glm::cos(glm::radians(spotLight->getOuterCutOff())));
                _shader->uniform1f("spotLights[" + std::to_string(index) + "]._constant",    spotLight->getConstant());
                _shader->uniform1f("spotLights[" + std::to_string(index) + "]._linear",      spotLight->getLinear());
                _shader->uniform1f("spotLights[" + std::to_string(index) + "]._quadratic",   spotLight->getQuadratic());

                _shader->uniform3f("spotLights[" + std::to_string(index) + "]._ambient",     spotLight->getAmbient());
                _shader->uniform3f("spotLights[" + std::to_string(index) + "]._diffuse",     spotLight->getDiffuse());
                _shader->uniform3f("spotLights[" + std::to_string(index) + "]._specular",    spotLight->getSpecular());
                index += 1;
            }
            index = 0;

            _shader->uniform1i("directionLightCount", world.getDirectionLights().size());
            for (const auto & directionLight : world.getDirectionLights()) {
                _shader->uniform3f("directionLights[" + std::to_string(index) + "]._direction",  directionLight->getDirection());

                _shader->uniform3f("directionLights[" + std::to_string(index) + "]._ambient",   directionLight->getAmbient());
                _shader->uniform3f("directionLights[" + std::to_string(index) + "]._diffuse",   directionLight->getDiffuse());
                _shader->uniform3f("directionLights[" + std::to_string(index) + "]._specular",  directionLight->getSpecular());
                index += 1;
            }
            index = 0;

            int id = 0;
            _shader->uniform1i("materialDiffuseCount", mesh->getTexturesDiffuse().size());
            for (const auto & texture : mesh->getTexturesDiffuse()) {
                texture->bindTextureDiffuse(*_shader, id, index);
                id += 1;
                index += 1;
            }
            index = 0;
            _shader->uniform1i("materialSpecularCount", mesh->getTexturesSpecular().size());
            for (const auto & texture : mesh->getTexturesSpecular()) {
                texture->bindTextureSpecular(*_shader, id, index);
                id += 1;
                index += 1;
            }
            index = 0;
            _shader->uniform1i("materialEmbientCount", mesh->getTexturesEmbient().size());
            for (const auto & texture : mesh->getTexturesEmbient()) {
                texture->bindTextureEmbient(*_shader, id, index);
                id += 1;
                index += 1;
            }
            
            glDrawElements(GL_TRIANGLES  , mesh->getIndicesCount(), GL_UNSIGNED_INT, 0);
        }
    }

    _window->swapBuffers();
    glfwPollEvents();
}  