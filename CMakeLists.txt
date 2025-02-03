cmake_minimum_required(VERSION 3.20)
project(Engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT DEFINED(${CMAKE_BUILD_TYPE}))
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Debug")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_SYSTEM_NAME}/${CMAKE_BUILD_TYPE}/bin")

include(FetchContent)
set(FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/external")

# GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
    GIT_PROGRESS TRUE
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw)


# Assimp (optimized for OBJ and FBX)
FetchContent_Declare(
    assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG v5.4.2
    GIT_PROGRESS TRUE
)

# Optimize Assimp build

set(ASSIMP_BUILD_ASSIMP_TOOLS             OFF CACHE BOOL "Disable Assimp tools"             FORCE)
set(ASSIMP_BUILD_TESTS                    OFF CACHE BOOL "Disable Assimp tests"             FORCE)
set(ASSIMP_NO_EXPORT                      ON  CACHE BOOL "Disable export functionality"     FORCE)

FetchContent_MakeAvailable(assimp)


# GLM
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 0.9.9.8
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(glm)


add_executable(${PROJECT_NAME}
    src/main.cpp 
    src/Shaders/Shader.cpp
    src/io/Window.cpp
    src/io/Keyboard.cpp
    src/io/Mouse.cpp
    src/io/InputObjects.cpp
    src/Object.cpp
    src/Mesh.cpp
    src/Model.cpp
    src/Texture.cpp
    src/Renderer.cpp
    src/Camera.cpp
    src/ObjectController.cpp
    src/Utilities/Time.cpp
    src/Utilities/ResourceManager.cpp
    src/Lighting/Light.cpp
    src/Lighting/PointLight.cpp
    src/Lighting/DirectionLight.cpp
    src/Lighting/SpotLight.cpp
    src/World.cpp
    glad/src/glad.c
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/glad/include
    ${glm_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}
)

find_package(OpenGL REQUIRED)

if(OPENGL_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        glfw
        assimp
        OpenGL::GL
    )
else()
    message(FATAL_ERROR "OpenGL not found!")
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
)



