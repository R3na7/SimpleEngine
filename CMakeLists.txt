cmake_minimum_required(VERSION 3.20)
project(Engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build" FORCE)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/bin")

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

set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "Disable all importers" FORCE)
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "Enable OBJ importer" FORCE)
set(ASSIMP_BUILD_FBX_IMPORTER ON CACHE BOOL "Enable FBX importer" FORCE)

set(ASSIMP_BUILD_DRACO OFF CACHE BOOL "Disable Draco compression" FORCE)
set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "Enable Assimp's internal Zlib" FORCE)
set(ASSIMP_BUILD_IRR_COMPILE_WITH_IRR_LMTS OFF CACHE BOOL "Disable IRR Lightmaps" FORCE)
set(ASSIMP_BUILD_NONFREE_C4D_IMPORTER OFF CACHE BOOL "Disable non-free Cinema4D importer" FORCE)

set(ASSIMP_BUILD_NO_MDL OFF CACHE BOOL "Disable MDL" FORCE)
set(ASSIMP_BUILD_NO_3D OFF CACHE BOOL "Disable 3D" FORCE)
set(ASSIMP_BUILD_NO_IFC OFF CACHE BOOL "Disable IFC" FORCE)
set(ASSIMP_BUILD_NO_X3D OFF CACHE BOOL "Disable X3D" FORCE)

set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "Disable treating warnings as errors" FORCE)
set(ASSIMP_NO_VALIDATEDATA OFF CACHE BOOL "Disable validation data" FORCE)
set(ASSIMP_BUILD_NO_EXPORT ON CACHE BOOL "Disable exporters" FORCE)

FetchContent_MakeAvailable(assimp)


# GLM
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 0.9.9.8
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(glm)


#stb_image.h
file(DOWNLOAD "https://raw.githubusercontent.com/nothings/stb/master/stb_image.h" 
             "${CMAKE_SOURCE_DIR}/external/stb_image/stb_image.h")


#GLAD
set(GLAD_URL "https://glad.dav1d.de/generated/tmpp_t79s5sglad/glad.zip")
set(GLAD_ZIP "${CMAKE_SOURCE_DIR}/external/glad.zip")
set(GLAD_DIR "${CMAKE_SOURCE_DIR}/external/glad")

if(NOT EXISTS "${GLAD_DIR}")
    file(DOWNLOAD ${GLAD_URL} ${GLAD_ZIP} SHOW_PROGRESS)
    file(ARCHIVE_EXTRACT INPUT ${GLAD_ZIP} DESTINATION ${GLAD_DIR})
endif()

add_library(glad STATIC "${GLAD_DIR}/src/glad.c")
target_include_directories(glad PUBLIC "${GLAD_DIR}/include")


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
)

target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_SOURCE_DIR}/external"
    "${glm_SOURCE_DIR}"
)

find_package(OpenGL REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
    glad
    glfw
    assimp
    OpenGL::GL
)


if(EXISTS "${CMAKE_SOURCE_DIR}/assets")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets"
    )
endif()



