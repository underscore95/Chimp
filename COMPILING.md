# Compiling

## How is the engine linked to the game?

The engine is an executable and the game is a statically linked library.
Chimp (the engine) and ChimpGame (the game) are separate projects.

## PC Requirements

- 64-bit Windows
- GPU supports at least OpenGL 4.6

## How do I build it then?

- Install all dependencies using a package manager (I use vcpkg)
- `vcpkg install imgui[opengl3-binding,glfw-binding] assimp opengl openal glfw enet flecs stb libsndfile glm`
- Set cacheVariables.CMAKE_TOOLCHAIN_FILE in CMakePresets.json to package manager path.
- Set VCPKG_ROOT in environment variables

Simply use visual studio to run the "Chimp.exe" target.
You may need to generate the build configurations prior to this, which can be done by reloading both CMakeLists.txt files.