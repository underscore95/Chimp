# Compiling

Chimp will automatically download and build all required dependencies.
This does mean your first build of Chimp will be slow and may fail on unstable internet connections.
Currently you can only compile chimp on windows (it uses .bat scripts) and you must have git and cmake in your environment path.
Edit Chimp/clone_lib.bat and Chimp/enet_build.bat if you don't have them in your path.

## How is the engine linked to the game?

The engine is an executable and the game is a statically linked library.
Chimp (the engine) and ChimpGame (the game) are separate projects.

## PC Requirements

- 64-bit Windows
- GPU supports at least OpenGL 4.6

## How do I build it then?

Ensure git and cmake are in your environment path.
Ensure you have a stable internet connection.

Simply use visual studio to run the "Chimp.exe" target.
You may need to generate the build configurations prior to this, which can be done by reloading both CMakeLists.txt files.

If CMake fails to execute clone_libs.bat, you will need to manually clone the git repos of header-only libraries into the vendor folder.
See CMakeLists.txt for Chimp (engine) for the git repositories and where they should be cloned to.