# Compiling

Make sure none of the parent folders of the repository are called "Data"

## PC Requirements

- 64-bit Windows
- GPU supports at least OpenGL 4.6
- An audio device

## How do I build it then?

- Install all dependencies using a package manager (I use vcpkg)
- `vcpkg install imgui[opengl3-binding,glfw-binding] assimp opengl openal glfw enet flecs stb libsndfile glm`
- Set cacheVariables.CMAKE_TOOLCHAIN_FILE in CMakePresets.json to package manager path.
- Set VCPKG_ROOT in environment variables
- Reload the ChimpFramework CMakeLists.txt, you can do this in visual studio by saving it (ctrl s)
- Build ChimpEditor.exe

Simply use visual studio to run the "ChimpGame.exe" target.