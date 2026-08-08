# SimpleEngine

![C++](https://img.shields.io/badge/C%2B%2B-20-00599C?logo=c%2B%2B&logoColor=white)
![OpenGL](https://img.shields.io/badge/OpenGL-3.3-5586A4?logo=opengl&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-3.20%2B-064F8C?logo=cmake&logoColor=white)
[![C++ CI](https://github.com/R3na7/SimpleEngine/actions/workflows/build.yml/badge.svg)](https://github.com/R3na7/SimpleEngine/actions/workflows/build.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

SimpleEngine is an educational 3D rendering engine written in C++20 and OpenGL 3.3.
It demonstrates a compact real-time rendering pipeline, model loading, textured meshes,
Phong-style lighting, and interactive camera controls.

> [!NOTE]
> SimpleEngine is an educational project created while learning C++, OpenGL, 3D graphics,
> and resource management. It is not intended to be a production-ready game engine.

## Demo

### Demo scene

<!-- Add demo scene image here -->

The textured FBX backpack is positioned between the blue and green cubes. Each cube has an
associated point light with the same color.

### Dynamic lighting

<!-- Add dynamic lighting GIF/image here -->

The backpack is illuminated by the green cube while the blue cube moves through the scene
with its point light attached.

### Lighting close-up

<!-- Add lighting close-up image here -->

The close-up shows the blue and green diffuse/specular lighting on the backpack surface.

## Features

- Indexed triangle rendering with VAO, VBO, EBO, and depth testing
- Composite models made from one or more meshes
- OBJ and FBX model loading through Assimp
- Vertex positions, indices, normals, UV coordinates, and imported node transformations
- Diffuse and specular texture loading through `stb_image`
- Phong-style point, spot, and directional lighting
- Object translation, rotation, and scaling
- Perspective camera with keyboard and mouse controls
- Runtime world container for models, meshes, cameras, and lights
- Procedural cube geometry
- FPS display in the window title

Model import is intentionally limited to OBJ and FBX. The bundled demo uses the FBX path;
the spot and directional light implementations are available in the engine but are not used
by the default scene.

## Technologies

- C++20
- OpenGL 3.3 Core
- GLSL 3.30
- GLFW 3.4
- GLAD
- GLM 0.9.9.8
- Assimp 5.4.2
- `stb_image`
- CMake 3.20+

## Architecture

- **Renderer** prepares each frame, uploads camera, lighting, and material uniforms, and
  draws the models stored in the world.
- **World** owns typed collections of scene objects and keeps track of the active camera.
- **Model** groups meshes and shared textures under a single object transformation.
- **Mesh** stores vertex/index data and owns its OpenGL vertex array and buffer objects.
- **Texture** loads image data, creates an OpenGL texture, and manages its lifetime.
- **Shader** compiles GLSL source files, links the shader program, and provides uniform setters.
- **Camera** creates the projection and view matrices from its position, yaw, and pitch.
- **ResourceManager** imports OBJ and FBX scenes through Assimp and converts them into engine
  models and meshes.

## Project structure

```text
.
├── assets/
│   ├── models/          # Demo models and their textures
│   ├── shaders/         # GLSL shader sources
│   └── textures/        # Additional learning assets
├── src/
│   ├── Lighting/        # Point, spot, and directional lights
│   ├── Shaders/         # Shader program wrapper
│   ├── Utilities/       # Model loading and frame timing
│   └── io/              # Window, keyboard, and mouse input
├── third_party/         # Vendored GLAD and stb_image sources
├── .github/workflows/   # Linux build workflow
└── CMakeLists.txt
```

## Quick Start

SimpleEngine is built as a demo executable rather than an installed library. The following
minimal `src/main.cpp` uses the existing engine classes to render one cube:

```cpp
#include "Renderer.hpp"
#include "Utilities/Time.hpp"

#include <iostream>

namespace {
int runDemo() {
    Window window(800, 600, "SimpleEngine quick start");
    if (!window.isOpen()) {
        return 1;
    }

    window.setBackgroundColor(0.2f, 0.2f, 0.2f, 1.0f);

    Renderer renderer(window);
    World world;

    auto cube = world.add(Model::getCube(
        {0.8f, 0.2f, 0.1f, 1.0f}, // color
        0.5f,                      // half-size
        "demoCube"                 // name
    ));
    cube->translate_to_point({0.0f, 0.0f, -3.0f});

    world.add(PointLight(
        {0.05f, 0.05f, 0.05f}, // ambient
        {0.8f, 0.8f, 0.8f},    // diffuse
        {1.0f, 1.0f, 1.0f},    // specular
        {2.0f, 2.0f, -1.0f}    // position
    ));

    Time::init();
    while (Window::havesWindow()) {
        renderer.render(world);
        Time::update();
    }

    return 0;
}
} // namespace

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const int result = runDemo();
    glfwTerminate();
    return result;
}
```

Result: an 800 x 600 window with an orange cube illuminated by a white point light. Press
`Escape` to close it. `World` creates the default camera automatically, and every `Model`
added to it is drawn by `renderer.render(world)`.

## Implemented interface

| Area                   | Available interface                                                                               |
| ---------------------- | ------------------------------------------------------------------------------------------------- |
| Scene management       | `World::add`, `remove`, `at`, `operator[]`, active-camera selection, and typed object collections |
| Built-in geometry      | `Model::getCube` and `Mesh::getCube`                                                              |
| Model loading          | `ResourceManager::loadModel` for `.obj`, `.fbx`, and `.FBX` files                                 |
| Object transforms      | `translate`, `translate_to_point`, `rotate`, axis rotations, `scale`, `Pitch`, and `Yaw`          |
| Textures and materials | Texture loading plus diffuse/specular textures, color, and shininess                              |
| Lighting               | Point lights, spotlights, and directional lights with their lighting parameters                   |
| Camera                 | Perspective projection, view matrix, position, yaw, pitch, and active-camera selection            |
| Rendering              | World rendering, camera/light/material uniforms, framebuffer clearing, and FPS calculation        |
| Window and input       | GLFW window/context management, keyboard state, mouse state, and `ObjectController`               |

This table lists only working parts of the current rendering path. Scene serialization,
line rendering, and the unfinished ambient/emission texture path are not included.

## Build

The build has been verified on Ubuntu 24.04. CMake downloads GLFW, Assimp, and GLM during
configuration, so Git and an internet connection are required for the first build.

Install the required compiler, OpenGL, X11, and Wayland development packages on Ubuntu or
Debian:

```bash
sudo apt-get update
sudo apt-get install -y --no-install-recommends \
  build-essential cmake git ninja-build pkg-config \
  libgl1-mesa-dev \
  libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libxext-dev \
  libwayland-dev libxkbcommon-dev wayland-protocols
```

Clone and build the Release configuration:

```bash
git clone https://github.com/R3na7/SimpleEngine.git
cd SimpleEngine
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
```

Run the executable from the repository root so relative asset paths resolve correctly:

```bash
./build/Release/bin/Engine
```

The application requires a desktop session and an OpenGL 3.3-capable graphics driver.

## Controls

| Input                  | Action                                          |
| ---------------------- | ----------------------------------------------- |
| Mouse movement         | Rotate the active camera or selected cube       |
| `W` / `S`              | Move forward / backward on the horizontal plane |
| `A` / `D`              | Move left / right on the horizontal plane       |
| `Space` / `Left Shift` | Move up / down                                  |
| `Q` / `E`              | Rotate left / right                             |
| Hold `J`               | Control the green cube instead of the camera    |
| Hold `K`               | Control the blue cube instead of the camera     |
| `Escape`               | Close the application                           |

Moving either cube also moves its associated point light.

## What I learned

- How vertex and index data moves through the OpenGL rendering pipeline
- How to manage vertex arrays, buffers, textures, and shader programs in C++
- How to write GLSL vertex and fragment shaders
- How model, view, and projection matrices form a camera pipeline
- How to import meshes and materials with Assimp
- How diffuse and specular textures participate in Phong lighting
- How to connect GLFW input callbacks to interactive camera and object movement
- How resource lifetime and destruction order matter for OpenGL objects

## Limitations

- This is an educational renderer, not a production game engine.
- There is no editor, audio, animation/skinning, physics, collision detection, or networking.
- The material and lighting pipeline is based on classic Phong lighting; there is no PBR,
  shadow mapping, or normal mapping.
- Scene serialization is not implemented, and scene management remains intentionally basic.
- Model import is limited to external OBJ/FBX assets and their image files.

## Credits

The bundled [Survival Guitar Backpack](https://sketchfab.com/3d-models/survival-guitar-backpack-799f8c4511f84fab8c3f12887f7e6b36)
model was created by Berk Gedik and is licensed under
[CC BY 4.0](https://creativecommons.org/licenses/by/4.0/). See the complete
[asset attribution](assets/models/ATTRIBUTION.md).
