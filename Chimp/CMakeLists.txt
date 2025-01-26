cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Platforms
set(CHIMP_PLATFORM_UNSUPPORTED 0)
set(CHIMP_PLATFORM_WINDOWS 1)

set(CHIMP_PLATFORM ${CHIMP_PLATFORM_UNSUPPORTED})

if(WIN32)
    set(CHIMP_PLATFORM ${CHIMP_PLATFORM_WINDOWS})
    add_compile_definitions(CHIMP_PLATFORM=CHIMP_PLATFORM_WINDOWS)
endif()

# Chimp
project ("Chimp")

include(FetchContent)

add_subdirectory("ChimpGame")

# Sources
add_executable (Chimp "src/Main.cpp" "src/Chimp.h"  "src/api/scenes/Scene.h" "src/stdafx.h" "src/api/scenes/SceneManager.h" "src/impl/scenes/SceneManager.cpp" "src/impl/MainLoop.h" "src/impl/MainLoop.cpp" "src/api/Engine.h" "src/impl/time/TimeManager.cpp" "src/impl/Engine.cpp" "src/api/window/IWindow.h" "src/api/window/InputManager.h" "src/api/window/Keys.h" "src/impl/window/InputManager.cpp" "src/impl/window/WindowGLFW.h" "src/impl/window/IWindow.cpp" "src/impl/window/WindowGLFW.cpp" "src/api/utils/Maths.h"  "src/api/graphics/buffers/IBuffer.h" "src/api/graphics/IRenderingManager.h" "src/api/graphics/buffers/Usage.h" "src/api/utils/preprocessor/CopyAndMove.h" "src/impl/graphics/opengl/RenderingManager.h" "src/impl/graphics/opengl/RenderingManager.cpp" "src/impl/graphics/opengl/buffers/Buffer.h" "src/impl/graphics/opengl/buffers/Buffer.cpp" "src/impl/graphics/opengl/Translations.h"  "src/api/graphics/buffers/IElementArray.h"  "src/api/utils/RawArray.h" "src/impl/utils/RawArray.cpp" "src/impl/graphics/interfaces/buffers/IElementArray.cpp" "src/impl/graphics/opengl/buffers/ElementArray.h" "src/impl/graphics/opengl/buffers/ElementArray.cpp" "src/api/graphics/buffers/IElementArrayLayout.h" "src/impl/graphics/interfaces/buffers/IElementArrayLayout.cpp" "src/api/graphics/GraphicsType.h" "src/impl/graphics/opengl/buffers/ElementArrayLayout.cpp" "src/impl/graphics/interfaces/GraphicsType.cpp" "src/api/utils/FileReader.h" "src/impl/utils/FileReader.cpp"  "src/api/graphics/shaders/IShader.h" "src/api/graphics/shaders/IShaderCompiler.h" "src/api/graphics/shaders/ShaderTypes.h" "src/impl/utils/NotNullHack.h" "src/impl/graphics/opengl/shaders/ShaderCompiler.h" "src/impl/graphics/opengl/shaders/ShaderCompiler.cpp" "src/impl/graphics/opengl/shaders/Shader.h" "src/impl/graphics/opengl/shaders/Shader.cpp"   "src/api/graphics/IRenderer.h" "src/impl/graphics/opengl/Renderer.h" "src/impl/graphics/opengl/Renderer.cpp" "src/api/graphics/PrimitiveType.h" "src/impl/graphics/interfaces/PrimitiveType.cpp" "src/api/graphics/meshes/Mesh.h" "src/impl/graphics/meshes/Mesh.cpp" "src/impl/graphics/interfaces/IRenderer.cpp" "src/api/graphics/camera/CameraMatrices.h" "src/impl/utils/Maths.cpp" "src/impl/graphics/camera/CameraMatrices.cpp" "src/api/graphics/camera/Camera.h" "src/impl/graphics/camera/Camera.cpp" "src/api/graphics/camera/ICamera.h" "src/api/graphics/shaders/IShaderBuffers.h" "src/impl/graphics/interfaces/shaders/IShaderBuffers.cpp" "src/impl/graphics/opengl/shaders/ShaderBuffers.h" "src/api/graphics/images/IImageLoader.h" "src/impl/graphics/images/interfaces/IImageLoader.cpp" "src/impl/graphics/images/stb/ImageLoader.h" "src/impl/graphics/images/stb/ImageLoader.cpp"  "src/api/graphics/textures/ITexture.h" "src/api/graphics/textures/TextureSlot.h" "src/impl/graphics/interfaces/textures/ITexture.cpp" "src/impl/graphics/opengl/textures/Texture.h" "src/impl/graphics/opengl/textures/Texture.cpp" "src/api/graphics/textures/TextureProperties.h" "src/impl/graphics/interfaces/IRenderingManager.cpp" "src/impl/graphics/images/stb/STBInit.cpp" "src/api/graphics/meshes/TexturedQuad.h" "src/impl/graphics/meshes/TexturedQuad.cpp" "src/api/graphics/camera/controllers/DebugCameraController.h" "src/impl/graphics/camera/controllers/DebugCameraController.cpp" "src/impl/graphics/interfaces/shaders/IShader.cpp" "src/impl/graphics/interfaces/buffers/IBuffer.cpp"  "src/api/resources/ResourceManager.h" "src/impl/resources/ResourceManager.cpp" "src/api/utils/HashCombine.h"  "src/api/ecs/ECS.h"  "src/api/utils/OptionalReference.h"  "src/api/networking/IServer.h"  "src/api/networking/EventHandler.h" "src/api/networking/events/NetworkEventType.h" "src/impl/networking/enet/Server.h" "src/impl/networking/enet/Server.cpp" "src/impl/networking/enet/InitENet.h" "src/impl/networking/enet/InitENet.cpp"  "src/impl/networking/enet/ServerFactory.h" "src/impl/networking/enet/ServerFactory.cpp" "src/impl/networking/IServer.cpp" "src/api/threading/ThreadSafeQueue.h"  "src/api/networking/IClient.h" "src/impl/networking/IClient.cpp" "src/impl/networking/enet/Client.h" "src/impl/networking/enet/Client.cpp" "src/api/networking/PacketTypeRegistry.h"  "src/api/networking/PacketResponseFunc.h" "src/api/logging/Logger.h" "src/impl/logging/Logger.cpp" "src/Loggers.h"  "src/api/imgui/ImGuiHandler.h" "src/impl/imgui/ImGuiHandler.cpp"  "src/api/utils/UpdateSubscriber.h" "src/impl/utils/UpdateSubscriber.cpp" "src/api/ecs/components/TransformComponent.h" "src/api/ecs/components/MeshComponent.h" "src/impl/scenes/Scene.cpp" "src/api/utils/Path.h" "src/api/utils/waves/WaveManager.h" "src/api/utils/waves/Wave.h" "src/api/utils/waves/WaveCommand.h" "src/impl/utils/waves/Wave.cpp" "src/impl/utils/waves/WaveCommand.cpp" "src/impl/utils/waves/WaveManager.cpp" "src/api/resources/IModelImporter.h" "src/impl/resources/assimp/ModelImporter.h" "src/impl/resources/assimp/ModelImporter.cpp" "src/api/utils/FilePath.h" "src/impl/utils/FilePath.cpp" "src/api/resources/MeshStorage.h" "src/impl/resources/MeshStorage.cpp" "src/api/threading/ThreadPool.h"  "src/api/utils/InPlaceOptional.h" "src/impl/threading/ThreadPool.cpp" "src/api/utils/TaskScheduler.h"  "src/impl/utils/TaskScheduler.cpp" "src/api/time/Timer.h" "src/impl/time/Timer.cpp" "src/api/utils/ImGuiHelper.h" "src/impl/utils/ImGuiHelper.cpp" "src/api/resources/Resource.h" "src/api/resources/ResourceContainer.h"  "src/api/resources/containers/ShaderResourceContainer.h" "src/impl/resources/containers/ShaderResourceContainer.cpp" "src/api/resources/containers/TextureResourceContainer.h" "src/impl/resources/containers/TextureResourceContainer.cpp"  "src/api/resources/ResourceDependency.h" "src/api/resources/containers/ModelResourceContainer.h" "src/impl/resources/containers/ModelResourceContainer.cpp" "src/api/resources/containers/SpriteResourceContainer.h" "src/impl/resources/containers/SpriteResourceContainer.cpp" "src/api/graphics/GameShader.h" "src/impl/graphics/GameShader.cpp" "src/api/ecs/Components.h" "src/api/files/yaml/YAMLBlockParser.h" "src/impl/files/yaml/YAMLBlockParser.cpp" "src/api/files/yaml/YAMLBlock.h" "src/api/files/yaml/YAMLWriter.h" "src/impl/files/yaml/YAMLWriter.cpp" "src/api/files/yaml/YAMLSerialiser.h" "src/impl/files/yaml/YAMLSerialiser.cpp" "src/api/utils/preprocessor/Casting.h" "src/api/files/yaml/YAMLSerialisable.h" "src/api/resources/containers/ImageResourceContainer.h" "src/impl/resources/containers/ImageResourceContainer.cpp" "src/api/utils/Random.h" "src/api/audio/ISound.h"  "src/api/audio/IListener.h" "src/impl/audio/openal/Listener.h"  "src/impl/audio/openal/Sound.h" "src/api/audio/IAudioImporter.h"  "src/api/audio/IPlayingAudio.h" "src/api/audio/AudioFormat.h" "src/impl/audio/openal/PlayingAudio.h" "src/api/audio/AudioManager.h" "src/impl/audio/AudioManager.cpp" "src/impl/audio/libsndfile/AudioImporter.h" "src/api/audio/IAudio.h" "src/impl/audio/openal/Audio.h" "src/api/resources/containers/SoundResourceContainer.h" "src/impl/resources/containers/SoundResourceContainer.cpp" "src/impl/audio/music/MusicTracksContainer.cpp" "src/impl/audio/music/MusicTracksContainer.cpp" "src/api/audio/music/MusicPlayer.h" "src/impl/audio/music/MusicPlayer.cpp" "src/api/audio/sfx/SoundEffectSettings.h" "src/api/resources/ResourcePaths.h" "src/impl/audio/sfx/SoundEffectSettings.cpp" "src/api/audio/sfx/SoundEffect.h" "src/impl/audio/sfx/SoundEffect.cpp" "src/api/resources/containers/SoundEffectResourceContainer.h" "src/impl/resources/containers/SoundEffectResourceContainer.cpp")

# Include directories
target_include_directories(Chimp PRIVATE "src/")
target_include_directories(Chimp PRIVATE "src/api/")
target_include_directories(Chimp PRIVATE "src/impl/")

target_link_libraries(Chimp PRIVATE ChimpGame)

# libsndfile
add_compile_definitions(CHIMP_LIBSNDFILE=1)
set(LIBSNDFILE_BUILD_TYPE "Debug")
if(CMAKE_BUILD_TYPE STREQUAL "Release")
	set(LIBSNDFILE_BUILD_TYPE "Release")
endif()
execute_process(
	COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/clone_lib.bat libsndfile/libsndfile-src https://github.com/libsndfile/libsndfile.git 1.2.2
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
execute_process(
	COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/libsndfile_build.bat ${LIBSNDFILE_BUILD_TYPE}
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/vendor/libsndfile
)
set(LIBSNDFILE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendor/libsndfile")
set(LIBSNDFILE_LIB "${LIBSNDFILE_DIR}/libsndfile-build/${LIBSNDFILE_BUILD_TYPE}/sndfile.lib")
target_link_libraries(Chimp PRIVATE ${LIBSNDFILE_LIB})
target_include_directories(Chimp PRIVATE ${LIBSNDFILE_DIR}/libsndfile-src/include)

# GLM
add_compile_definitions(CHIMP_GLM=1)
execute_process(
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/clone_lib.bat glm https://github.com/g-truc/glm.git 1.0.1
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

# STB
add_compile_definitions(CHIMP_STB=1)
execute_process(
	COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/clone_lib.bat stb https://github.com/nothings/stb.git 5c205738c191bcb0abc65c4febfa9bd25ff35234
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

# FLECS
add_compile_definitions(CHIMP_FLECS=1)
FetchContent_Declare(
    flecs
    GIT_REPOSITORY https://github.com/SanderMertens/flecs.git
    GIT_TAG b3ad03c391a268234c54270ccf3793c5b6ad6312
)
FetchContent_MakeAvailable(flecs)
target_link_libraries(Chimp PRIVATE flecs::flecs_static)

# ENET
# Ended up having a lot of issues with linking ENET
# This code basically clones the repo, builds it with cmake from a batch script, then links it
# there is definitely a better way to do this, but it works
# you do need cmake and git in your env path though
# also it uses batch so thats windows only
add_compile_definitions(CHIMP_ENET=1)
set(ENET_BUILD_TYPE "Debug")
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(ENET_BUILD_TYPE "Release")
endif()
execute_process(
	COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/clone_lib.bat enet/enet-src https://github.com/lsalzman/enet.git master
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
execute_process(
	COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/enet_build.bat ${ENET_BUILD_TYPE}
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/vendor/enet
)
set(ENET_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendor/enet")
include_directories(${ENET_DIR}/enet-src/include)
set(ENET_LIB "${ENET_DIR}/enet-build/${ENET_BUILD_TYPE}/enet.lib")
target_link_libraries(Chimp PRIVATE ${ENET_LIB} winmm)

# GLFW
add_compile_definitions(CHIMP_GLFW=1)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw)
target_link_libraries(Chimp PRIVATE glfw)

# OpenAL
add_compile_definitions(CHIMP_OPENAL=1)
FetchContent_Declare(
  openal
  GIT_REPOSITORY https://github.com/kcat/openal-soft.git
  GIT_TAG 1.24.1
)
FetchContent_MakeAvailable(openal)
target_link_libraries(Chimp PRIVATE OpenAL::OpenAL)
# Copy OpenAL dll into build directory, since apparantly that is necessary
set(COPY_DLL_SCRIPT ${CMAKE_BINARY_DIR}/copy_openal_dlls.cmake)
file(WRITE ${COPY_DLL_SCRIPT} "
if(EXISTS \"${CMAKE_BINARY_DIR}/_deps/openal-build/OpenAL32.dll\")
    file(COPY \"${CMAKE_BINARY_DIR}/_deps/openal-build/OpenAL32.dll\" DESTINATION \"${CMAKE_BINARY_DIR}\")
endif()
if(EXISTS \"${CMAKE_BINARY_DIR}/_deps/openal-build/OpenAL32d.dll\")
    file(COPY \"${CMAKE_BINARY_DIR}/_deps/openal-build/OpenAL32d.dll\" DESTINATION \"${CMAKE_BINARY_DIR}\")
endif()
")
add_custom_command(TARGET Chimp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -P ${COPY_DLL_SCRIPT}
)

# OpenGL
add_compile_definitions(CHIMP_OPENGL=1)
find_package(OpenGL REQUIRED)
target_link_libraries(Chimp PRIVATE OpenGL::GL)
# GLEW (OpenGL Extension Wrangler Library)
FetchContent_Declare(
  glew
  GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
  GIT_TAG glew-cmake-2.2.0
)
FetchContent_MakeAvailable(glew)
target_link_libraries(Chimp PRIVATE libglew_static)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Chimp PROPERTY CXX_STANDARD 20)
endif()

## Assimp
add_compile_definitions(CHIMP_ASSIMP=1)
FetchContent_Declare(
    Assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG v5.4.3 
)
FetchContent_MakeAvailable(Assimp)
target_link_libraries(Chimp PRIVATE assimp)

# ImGui
add_compile_definitions(CHIMP_IMGUI=1)
execute_process(
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/clone_lib.bat imgui https://github.com/ocornut/imgui.git master
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
set(IMGUI_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui)
file(GLOB IMGUI_SRC
    ${IMGUI_SOURCE_DIR}/*.cpp
    ${IMGUI_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
add_library(imgui STATIC ${IMGUI_SRC})
target_include_directories(imgui PUBLIC
    ${IMGUI_SOURCE_DIR}
    ${IMGUI_SOURCE_DIR}/backends
)
find_package(OpenGL REQUIRED)
target_link_libraries(imgui PUBLIC glfw OpenGL::GL)
target_link_libraries(Chimp PRIVATE imgui)