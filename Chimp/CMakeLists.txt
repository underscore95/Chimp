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

add_subdirectory("ChimpGame")

# Sources
add_executable (Chimp "src/Main.cpp" "src/Chimp.h"  "src/api/scenes/Scene.h" "src/stdafx.h" "src/api/scenes/SceneManager.h" "src/impl/scenes/SceneManager.cpp" "src/impl/MainLoop.h" "src/impl/MainLoop.cpp" "src/api/Engine.h" "src/impl/time/TimeManager.cpp" "src/impl/Engine.cpp" "src/api/window/IWindow.h" "src/api/window/InputManager.h" "src/api/window/Keys.h" "src/impl/window/InputManager.cpp" "src/impl/window/WindowGLFW.h" "src/impl/window/IWindow.cpp" "src/impl/window/WindowGLFW.cpp" "src/api/utils/Maths.h"  "src/api/graphics/buffers/IBuffer.h" "src/api/graphics/IRenderingManager.h" "src/api/graphics/buffers/Usage.h" "src/api/utils/preprocessor/CopyAndMove.h" "src/impl/graphics/opengl/RenderingManager.h" "src/impl/graphics/opengl/RenderingManager.cpp" "src/impl/graphics/opengl/buffers/Buffer.h" "src/impl/graphics/opengl/buffers/Buffer.cpp" "src/impl/graphics/opengl/Translations.h"  "src/api/graphics/buffers/IElementArray.h"  "src/api/utils/RawArray.h" "src/impl/utils/RawArray.cpp" "src/impl/graphics/interfaces/buffers/IElementArray.cpp" "src/impl/graphics/opengl/buffers/ElementArray.h" "src/impl/graphics/opengl/buffers/ElementArray.cpp" "src/api/graphics/buffers/IElementArrayLayout.h" "src/impl/graphics/interfaces/buffers/IElementArrayLayout.cpp" "src/api/graphics/GraphicsType.h" "src/impl/graphics/opengl/buffers/ElementArrayLayout.cpp" "src/impl/graphics/interfaces/GraphicsType.cpp" "src/api/utils/FileReader.h" "src/impl/utils/FileReader.cpp"  "src/api/graphics/shaders/IShader.h" "src/api/graphics/shaders/IShaderCompiler.h" "src/api/graphics/shaders/ShaderTypes.h" "src/impl/utils/NotNullHack.h" "src/impl/graphics/opengl/shaders/ShaderCompiler.h" "src/impl/graphics/opengl/shaders/ShaderCompiler.cpp" "src/impl/graphics/opengl/shaders/Shader.h" "src/impl/graphics/opengl/shaders/Shader.cpp"   "src/api/graphics/IRenderer.h" "src/impl/graphics/opengl/Renderer.h" "src/impl/graphics/opengl/Renderer.cpp" "src/api/graphics/PrimitiveType.h" "src/impl/graphics/interfaces/PrimitiveType.cpp" "src/api/graphics/meshes/Mesh.h" "src/impl/graphics/meshes/Mesh.cpp" "src/impl/graphics/interfaces/IRenderer.cpp" "src/api/graphics/camera/CameraMatrices.h" "src/impl/utils/Maths.cpp" "src/impl/graphics/camera/CameraMatrices.cpp" "src/api/graphics/camera/Camera.h" "src/impl/graphics/camera/Camera.cpp" "src/api/graphics/camera/ICamera.h" "src/api/graphics/shaders/IShaderBuffers.h" "src/impl/graphics/interfaces/shaders/IShaderBuffers.cpp" "src/impl/graphics/opengl/shaders/ShaderBuffers.h" "src/api/graphics/images/IImageLoader.h" "src/impl/graphics/images/interfaces/IImageLoader.cpp" "src/impl/graphics/images/stb/ImageLoader.h" "src/impl/graphics/images/stb/ImageLoader.cpp"  "src/api/graphics/textures/ITexture.h" "src/api/graphics/textures/TextureSlot.h" "src/impl/graphics/interfaces/textures/ITexture.cpp" "src/impl/graphics/opengl/textures/Texture.h" "src/impl/graphics/opengl/textures/Texture.cpp" "src/api/graphics/textures/TextureProperties.h" "src/impl/graphics/interfaces/IRenderingManager.cpp" "src/impl/graphics/images/stb/STBInit.cpp" "src/api/graphics/meshes/TexturedQuad.h" "src/impl/graphics/meshes/TexturedQuad.cpp" "src/api/graphics/camera/controllers/FreeCameraController.h" "src/impl/graphics/camera/controllers/FreeCameraController.cpp"  "src/impl/graphics/interfaces/buffers/IBuffer.cpp"  "src/api/resources/ResourceManager.h" "src/impl/resources/ResourceManager.cpp" "src/api/utils/HashCombine.h"  "src/api/ecs/ECS.h"  "src/api/utils/OptionalReference.h"  "src/api/networking/IServer.h"  "src/api/networking/EventHandler.h" "src/api/networking/events/NetworkEventType.h" "src/impl/networking/enet/Server.h" "src/impl/networking/enet/Server.cpp" "src/impl/networking/enet/InitENet.h" "src/impl/networking/enet/InitENet.cpp"  "src/impl/networking/enet/ServerFactory.h" "src/impl/networking/enet/ServerFactory.cpp" "src/impl/networking/IServer.cpp" "src/api/threading/ThreadSafeQueue.h"  "src/api/networking/IClient.h" "src/impl/networking/IClient.cpp" "src/impl/networking/enet/Client.h" "src/impl/networking/enet/Client.cpp" "src/api/networking/PacketTypeRegistry.h"  "src/api/networking/PacketResponseFunc.h" "src/api/logging/Logger.h" "src/impl/logging/Logger.cpp" "src/Loggers.h"  "src/api/imgui/ImGuiHandler.h" "src/impl/imgui/ImGuiHandler.cpp"  "src/api/utils/UpdateSubscriber.h" "src/impl/utils/UpdateSubscriber.cpp" "src/api/ecs/components/TransformComponent.h" "src/api/ecs/components/MeshComponent.h" "src/impl/scenes/Scene.cpp" "src/api/utils/Path.h" "src/api/utils/waves/WaveManager.h" "src/api/utils/waves/Wave.h" "src/api/utils/waves/WaveCommand.h" "src/impl/utils/waves/Wave.cpp" "src/impl/utils/waves/WaveCommand.cpp" "src/impl/utils/waves/WaveManager.cpp" "src/api/resources/IModelImporter.h" "src/impl/resources/assimp/ModelImporter.h" "src/impl/resources/assimp/ModelImporter.cpp" "src/api/utils/FilePath.h" "src/impl/utils/FilePath.cpp" "src/api/resources/MeshStorage.h" "src/impl/resources/MeshStorage.cpp" "src/api/threading/ThreadPool.h"  "src/api/utils/InPlaceOptional.h" "src/impl/threading/ThreadPool.cpp" "src/api/utils/TaskScheduler.h"  "src/impl/utils/TaskScheduler.cpp" "src/api/time/Timer.h" "src/impl/time/Timer.cpp" "src/api/utils/ImGuiHelper.h" "src/impl/utils/ImGuiHelper.cpp" "src/api/resources/Resource.h" "src/api/resources/ResourceContainer.h"  "src/api/resources/containers/ShaderResourceContainer.h" "src/impl/resources/containers/ShaderResourceContainer.cpp" "src/api/resources/containers/TextureResourceContainer.h" "src/impl/resources/containers/TextureResourceContainer.cpp"  "src/api/resources/ResourceDependency.h" "src/api/resources/containers/ModelResourceContainer.h" "src/impl/resources/containers/ModelResourceContainer.cpp" "src/api/resources/containers/SpriteResourceContainer.h" "src/impl/resources/containers/SpriteResourceContainer.cpp" "src/api/graphics/GameShader.h" "src/impl/graphics/GameShader.cpp" "src/api/ecs/Components.h" "src/api/utils/preprocessor/Casting.h" "src/api/resources/containers/ImageResourceContainer.h" "src/impl/resources/containers/ImageResourceContainer.cpp" "src/api/utils/Random.h" "src/api/audio/ISound.h"  "src/api/audio/IListener.h" "src/impl/audio/openal/Listener.h"  "src/impl/audio/openal/Sound.h" "src/api/audio/IAudioImporter.h"  "src/api/audio/IPlayingAudio.h" "src/api/audio/AudioFormat.h" "src/impl/audio/openal/PlayingAudio.h" "src/api/audio/AudioManager.h" "src/impl/audio/AudioManager.cpp" "src/impl/audio/libsndfile/AudioImporter.h" "src/api/audio/IAudio.h" "src/impl/audio/openal/Audio.h" "src/api/resources/containers/SoundResourceContainer.h" "src/impl/resources/containers/SoundResourceContainer.cpp" "src/impl/audio/music/MusicTracksContainer.cpp" "src/impl/audio/music/MusicTracksContainer.cpp" "src/api/audio/music/MusicPlayer.h" "src/impl/audio/music/MusicPlayer.cpp" "src/api/audio/sfx/SoundEffectSettings.h" "src/api/resources/ResourcePaths.h" "src/impl/audio/sfx/SoundEffectSettings.cpp" "src/api/audio/sfx/SoundEffect.h" "src/impl/audio/sfx/SoundEffect.cpp" "src/api/graphics/shaders/shaders/ChimpShaders.h" "src/impl/graphics/shaders/shaders/ChimpShaders.cpp" "src/api/graphics/shaders/shaders/LitShader.h" "src/impl/graphics/shaders/shaders/LitShader.cpp"  "src/api/graphics/shaders/shaders/lighting/Lights.h" "src/api/graphics/camera/CameraPerspective.h" "src/impl/graphics/camera/CameraPerspective.cpp" "src/api/graphics/shadows/IShadowMap.h" "src/impl/graphics/opengl/shadows/ShadowMap.h" "src/impl/graphics/opengl/shadows/ShadowMap.cpp"  "src/impl/graphics/interfaces/shaders/IShader.cpp" "src/impl/graphics/opengl/shadows/CubeShadowMap.h" "src/impl/graphics/opengl/shadows/CubeShadowMap.cpp")

# Include directories
target_include_directories(Chimp PRIVATE "src/")
target_include_directories(Chimp PRIVATE "src/api/")
target_include_directories(Chimp PRIVATE "src/impl/")

target_link_libraries(Chimp PRIVATE ChimpGame)

# GLM
add_compile_definitions(CHIMP_GLM=1)
find_package(glm CONFIG REQUIRED)
target_link_libraries(Chimp PRIVATE glm::glm)

# libsndfile
add_compile_definitions(CHIMP_LIBSNDFILE=1)
find_package(SndFile CONFIG REQUIRED)
target_link_libraries(Chimp PRIVATE SndFile::sndfile)

# STB
add_compile_definitions(CHIMP_STB=1)
find_package(Stb REQUIRED)
target_include_directories(Chimp PRIVATE ${Stb_INCLUDE_DIR})

# FLECS
add_compile_definitions(CHIMP_FLECS=1)
find_package(flecs CONFIG REQUIRED)
target_link_libraries(Chimp PRIVATE $<IF:$<TARGET_EXISTS:flecs::flecs>,flecs::flecs,flecs::flecs_static>)

# ENET
add_compile_definitions(CHIMP_ENET=1)
find_package(unofficial-enet CONFIG REQUIRED)
target_link_libraries(Chimp PRIVATE unofficial::enet::enet winmm)

# GLFW
add_compile_definitions(CHIMP_GLFW=1)
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(Chimp PRIVATE glfw)

# OpenAL
add_compile_definitions(CHIMP_OPENAL=1)

find_package(OpenAL CONFIG REQUIRED)
target_link_libraries(Chimp PRIVATE OpenAL::OpenAL)

# OpenGL
add_compile_definitions(CHIMP_OPENGL=1)
find_package(OpenGL REQUIRED)
target_link_libraries(Chimp PRIVATE OpenGL::GL)
# GLEW (OpenGL Extension Wrangler Library)
find_package(GLEW REQUIRED)
target_link_libraries(Chimp PRIVATE GLEW::GLEW)

## Assimp
add_compile_definitions(CHIMP_ASSIMP=1)

find_package(assimp CONFIG REQUIRED)
target_link_libraries(Chimp PRIVATE assimp::assimp)

# ImGui
add_compile_definitions(CHIMP_IMGUI=1)

find_package(imgui CONFIG REQUIRED)

target_link_libraries(Chimp PRIVATE imgui::imgui)

# C++ Version

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Chimp PROPERTY CXX_STANDARD 20)
endif()

# In debug mode, we'll just use the data folder in the source directory
# in release mode, we'll copy it to the build directory and use that one
if (CMAKE_BUILD_TYPE STREQUAL "Release")
add_compile_definitions(CHIMP_DATA_FOLDER="ChimpData")
else()
add_compile_definitions(CHIMP_DATA_FOLDER="${CMAKE_CURRENT_SOURCE_DIR}/ChimpData")
endif()

# Copy data folder into build directory
# only copy in release
if (CMAKE_BUILD_TYPE STREQUAL "Release")
file(REMOVE_RECURSE ${CMAKE_BINARY_DIR}/ChimpData)
add_custom_command(TARGET Chimp POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_CURRENT_SOURCE_DIR}/ChimpData ${CMAKE_BINARY_DIR}/ChimpData
)
endif()