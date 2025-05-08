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

# Version
set(CHIMP_VERSION "2.2.0")

# Chimp
project ("Chimp")

add_subdirectory("../ChimpGame" "${CMAKE_BINARY_DIR}/ChimpGame")
add_subdirectory("../ChimpEditor" "${CMAKE_BINARY_DIR}/ChimpEditor")

# Sources
add_library(Chimp SHARED "Source/Main.cpp" "Source/Chimp.h"  "Source/api/scenes/Scene.h" "Source/stdafx.h" "Source/api/scenes/SceneManager.h" "Source/impl/scenes/SceneManager.cpp" "Source/impl/MainLoop.h" "Source/impl/MainLoop.cpp" "Source/api/Engine.h" "Source/impl/time/TimeManager.cpp" "Source/impl/Engine.cpp" "Source/api/window/IWindow.h" "Source/api/window/InputManager.h" "Source/api/window/Keys.h" "Source/impl/window/InputManager.cpp" "Source/impl/window/WindowGLFW.h" "Source/impl/window/IWindow.cpp" "Source/impl/window/WindowGLFW.cpp" "Source/api/utils/Maths.h"  "Source/api/graphics/buffers/IBuffer.h" "Source/api/graphics/IRenderingManager.h" "Source/api/graphics/buffers/Usage.h" "Source/api/utils/preprocessor/CopyAndMove.h" "Source/impl/graphics/opengl/RenderingManager.h" "Source/impl/graphics/opengl/RenderingManager.cpp" "Source/impl/graphics/opengl/buffers/Buffer.h" "Source/impl/graphics/opengl/buffers/Buffer.cpp" "Source/impl/graphics/opengl/Translations.h"  "Source/api/graphics/buffers/IElementArray.h"  "Source/api/utils/RawArray.h" "Source/impl/utils/RawArray.cpp" "Source/impl/graphics/interfaces/buffers/IElementArray.cpp" "Source/impl/graphics/opengl/buffers/ElementArray.h" "Source/impl/graphics/opengl/buffers/ElementArray.cpp" "Source/api/graphics/buffers/IElementArrayLayout.h" "Source/impl/graphics/interfaces/buffers/IElementArrayLayout.cpp" "Source/api/graphics/GraphicsType.h" "Source/impl/graphics/opengl/buffers/ElementArrayLayout.cpp" "Source/impl/graphics/interfaces/GraphicsType.cpp" "Source/api/utils/FileReader.h" "Source/impl/utils/FileReader.cpp"  "Source/api/graphics/shaders/IShader.h" "Source/api/graphics/shaders/IShaderCompiler.h" "Source/api/graphics/shaders/ShaderTypes.h" "Source/impl/utils/NotNullHack.h" "Source/impl/graphics/opengl/shaders/ShaderCompiler.h" "Source/impl/graphics/opengl/shaders/ShaderCompiler.cpp" "Source/impl/graphics/opengl/shaders/Shader.h" "Source/impl/graphics/opengl/shaders/Shader.cpp"   "Source/api/graphics/IRenderer.h" "Source/impl/graphics/opengl/Renderer.h" "Source/impl/graphics/opengl/Renderer.cpp" "Source/api/graphics/PrimitiveType.h" "Source/impl/graphics/interfaces/PrimitiveType.cpp" "Source/api/graphics/meshes/Mesh.h" "Source/impl/graphics/meshes/Mesh.cpp" "Source/impl/graphics/interfaces/IRenderer.cpp" "Source/api/graphics/camera/CameraMatrices.h" "Source/impl/utils/Maths.cpp" "Source/impl/graphics/camera/CameraMatrices.cpp" "Source/api/graphics/camera/Camera.h" "Source/impl/graphics/camera/Camera.cpp" "Source/api/graphics/camera/ICamera.h" "Source/api/graphics/shaders/IShaderBuffers.h" "Source/impl/graphics/interfaces/shaders/IShaderBuffers.cpp" "Source/impl/graphics/opengl/shaders/ShaderBuffers.h" "Source/api/graphics/images/IImageLoader.h" "Source/impl/graphics/images/interfaces/IImageLoader.cpp" "Source/impl/graphics/images/stb/ImageLoader.h" "Source/impl/graphics/images/stb/ImageLoader.cpp"  "Source/api/graphics/textures/ITexture.h" "Source/api/graphics/textures/TextureSlot.h" "Source/impl/graphics/interfaces/textures/ITexture.cpp" "Source/impl/graphics/opengl/textures/Texture.h" "Source/impl/graphics/opengl/textures/Texture.cpp" "Source/api/graphics/textures/TextureProperties.h" "Source/impl/graphics/interfaces/IRenderingManager.cpp" "Source/impl/graphics/images/stb/STBInit.cpp" "Source/api/graphics/meshes/TexturedQuad.h" "Source/impl/graphics/meshes/TexturedQuad.cpp" "Source/api/graphics/camera/controllers/FreeCameraController.h" "Source/impl/graphics/camera/controllers/FreeCameraController.cpp"  "Source/impl/graphics/interfaces/buffers/IBuffer.cpp"  "Source/api/resources/ResourceManager.h" "Source/impl/resources/ResourceManager.cpp" "Source/api/utils/HashCombine.h"  "Source/api/ecs/ECS.h"  "Source/api/utils/OptionalReference.h"  "Source/api/networking/IServer.h"  "Source/api/networking/EventHandler.h" "Source/api/networking/events/NetworkEventType.h" "Source/impl/networking/enet/Server.h" "Source/impl/networking/enet/Server.cpp" "Source/impl/networking/enet/InitENet.h" "Source/impl/networking/enet/InitENet.cpp"  "Source/impl/networking/enet/ServerFactory.h" "Source/impl/networking/enet/ServerFactory.cpp" "Source/impl/networking/IServer.cpp" "Source/api/threading/ThreadSafeQueue.h"  "Source/api/networking/IClient.h" "Source/impl/networking/IClient.cpp" "Source/impl/networking/enet/Client.h" "Source/impl/networking/enet/Client.cpp" "Source/api/networking/PacketTypeRegistry.h"  "Source/api/networking/PacketResponseFunc.h" "Source/api/logging/Logger.h" "Source/impl/logging/Logger.cpp" "Source/Loggers.h"  "Source/api/imgui/ImGuiHandler.h" "Source/impl/imgui/ImGuiHandler.cpp"  "Source/api/utils/UpdateSubscriber.h" "Source/impl/utils/UpdateSubscriber.cpp" "Source/api/ecs/transform/TransformComponent.h" "Source/api/ecs/components/MeshComponent.h" "Source/impl/scenes/Scene.cpp" "Source/api/utils/Path.h" "Source/api/utils/waves/WaveManager.h" "Source/api/utils/waves/Wave.h" "Source/api/utils/waves/WaveCommand.h" "Source/impl/utils/waves/Wave.cpp" "Source/impl/utils/waves/WaveCommand.cpp" "Source/impl/utils/waves/WaveManager.cpp" "Source/api/resources/IModelImporter.h" "Source/impl/resources/assimp/ModelImporter.h" "Source/impl/resources/assimp/ModelImporter.cpp" "Source/api/utils/FilePath.h" "Source/impl/utils/FilePath.cpp" "Source/api/resources/MeshStorage.h" "Source/impl/resources/MeshStorage.cpp" "Source/api/threading/ThreadPool.h"  "Source/api/utils/InPlaceOptional.h" "Source/impl/threading/ThreadPool.cpp" "Source/api/utils/TaskScheduler.h"  "Source/impl/utils/TaskScheduler.cpp" "Source/api/time/Timer.h" "Source/impl/time/Timer.cpp" "Source/api/utils/ImGuiHelper.h" "Source/impl/utils/ImGuiHelper.cpp" "Source/api/resources/Resource.h" "Source/api/resources/ResourceContainer.h"  "Source/api/resources/containers/ShaderResourceContainer.h" "Source/impl/resources/containers/ShaderResourceContainer.cpp" "Source/api/resources/containers/TextureResourceContainer.h" "Source/impl/resources/containers/TextureResourceContainer.cpp"  "Source/api/resources/ResourceDependency.h" "Source/api/resources/containers/ModelResourceContainer.h" "Source/impl/resources/containers/ModelResourceContainer.cpp" "Source/api/resources/containers/SpriteResourceContainer.h" "Source/impl/resources/containers/SpriteResourceContainer.cpp" "Source/api/graphics/GameShader.h" "Source/impl/graphics/GameShader.cpp" "Source/api/ecs/Components.h" "Source/api/utils/preprocessor/Casting.h" "Source/api/resources/containers/ImageResourceContainer.h" "Source/impl/resources/containers/ImageResourceContainer.cpp" "Source/api/utils/Random.h" "Source/api/audio/ISound.h"  "Source/api/audio/IListener.h" "Source/impl/audio/openal/Listener.h"  "Source/impl/audio/openal/Sound.h" "Source/api/audio/IAudioImporter.h"  "Source/api/audio/IPlayingAudio.h" "Source/api/audio/AudioFormat.h" "Source/impl/audio/openal/PlayingAudio.h" "Source/api/audio/AudioManager.h" "Source/impl/audio/AudioManager.cpp" "Source/impl/audio/libsndfile/AudioImporter.h" "Source/api/audio/IAudio.h" "Source/impl/audio/openal/Audio.h" "Source/api/resources/containers/SoundResourceContainer.h" "Source/impl/resources/containers/SoundResourceContainer.cpp" "Source/impl/audio/music/MusicTracksContainer.cpp" "Source/impl/audio/music/MusicTracksContainer.cpp" "Source/api/audio/music/MusicPlayer.h" "Source/impl/audio/music/MusicPlayer.cpp" "Source/api/audio/sfx/SoundEffectSettings.h" "Source/api/resources/ResourcePaths.h" "Source/impl/audio/sfx/SoundEffectSettings.cpp" "Source/api/audio/sfx/SoundEffect.h" "Source/impl/audio/sfx/SoundEffect.cpp" "Source/api/graphics/shaders/shaders/ChimpShaders.h" "Source/impl/graphics/shaders/shaders/ChimpShaders.cpp" "Source/api/graphics/shaders/shaders/LitShader.h" "Source/impl/graphics/shaders/shaders/LitShader.cpp"  "Source/api/graphics/shaders/shaders/lighting/Lights.h" "Source/api/graphics/camera/CameraPerspective.h" "Source/impl/graphics/camera/CameraPerspective.cpp" "Source/api/graphics/utils/IRenderTexture.h"  "Source/impl/graphics/opengl/utils/shadows/ShadowMap.h" "Source/impl/graphics/opengl/utils/shadows/ShadowMap.cpp"  "Source/impl/graphics/interfaces/shaders/IShader.cpp" "Source/impl/graphics/opengl/utils/shadows/CubeShadowMap.h" "Source/impl/graphics/opengl/utils/shadows/CubeShadowMap.cpp"  "Source/api/graphics/shaders/shaders/lit/LitPointShadowShader.h" "Source/impl/graphics/shaders/shaders/lit/LitPointShadowShader.cpp" "Source/api/graphics/shaders/shaders/lit/LitShadowShader.h" "Source/impl/graphics/shaders/shaders/lit/LitShadowShader.cpp" "Source/api/graphics/utils/shadows/ICubeShadowMap.h"  "Source/api/ecs/components/HierarchyComponent.h"  "Source/api/ecs/EntityId.h" "Source/api/utils/UnorderedCollection.h" "Source/api/ecs/transform/TransformManager.h" "Source/impl/ecs/transform/TransformManager.cpp" "Source/impl/graphics/opengl/utils/RenderTexture.h" "Source/impl/graphics/opengl/utils/RenderTexture.cpp" "Source/api/ecs/ISystem.h" "Source/api/ecs/SystemContainerSystem.h" "Source/impl/ecs/SystemContainerSystem.cpp" "Source/api/ecs/scripting/IEntityScript.h" "Source/api/ecs/scripting/ScriptableComponent.h" "Source/api/ecs/scripting/EntityScriptingSystem.h" "Source/impl/ecs/scripting/EntityScriptingSystem.cpp"   "Source/api/ecs/components/ComponentRegistry.h" "Source/impl/ecs/components/ComponentRegistry.cpp"  "Source/impl/ecs/ECS.cpp" "Source/api/utils/AnyReference.h"   "Source/api/ecs/RegisterComponents.h" "Source/api/utils/TypeInfo.h")

# Include directories
target_include_directories(Chimp PRIVATE "Source/")
target_include_directories(Chimp PRIVATE "Source/api/")
target_include_directories(Chimp PRIVATE "Source/impl/")

# Export all symbols
if (MSVC)
    target_compile_options(Chimp PRIVATE /DCHIMP_EXPORTS /Z7 /Od)
    set_target_properties(Chimp PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

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
add_compile_definitions(CHIMP_DATA_FOLDER="Data")
else()
add_compile_definitions(CHIMP_DATA_FOLDER="${CMAKE_CURRENT_SOURCE_DIR}/Data")
endif()

# Copy data folder into build directory
# only copy in release
if (CMAKE_BUILD_TYPE STREQUAL "Release")
file(REMOVE_RECURSE ${CMAKE_BINARY_DIR}/Data)
add_custom_command(TARGET Chimp POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_CURRENT_SOURCE_DIR}/Data ${CMAKE_BINARY_DIR}/Data
)
endif()