# Simple Engine

A simple 3D engine written using OpenGL

There is no sound, animation, physics, or shadow casting (the Phong lighting model is used).

Support for downloading 3D models of the format.FBX and .OBJ

## How do I build a project?

```sh
git clone https://github.com/R3na7/SimpleEngine.git
cd ./SimpleEngine
mkdir ./build
cd ./build
cmake -G "YOUR_GENERATOR" ..
cmake --build .
```

The result of the build will be located in the folder ./build/BUILD_TYPE/bin