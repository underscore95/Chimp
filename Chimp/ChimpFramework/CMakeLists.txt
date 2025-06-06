# Platforms
set(CHIMP_PLATFORM_UNSUPPORTED 0)
set(CHIMP_PLATFORM_WINDOWS 1)

set(CHIMP_PLATFORM ${CHIMP_PLATFORM_UNSUPPORTED})

if(WIN32)
    set(CHIMP_PLATFORM ${CHIMP_PLATFORM_WINDOWS})
    add_compile_definitions(CHIMP_PLATFORM=CHIMP_PLATFORM_WINDOWS)
endif()

# Version
set(CHIMP_VERSION "2.3.0")

# Chimp
include(ChimpFrameworkSources.cmake)

# C++ Version
set_property(TARGET ChimpFramework PROPERTY CXX_STANDARD 20)

# Include directories
target_include_directories(ChimpFramework PUBLIC "Source/")
target_include_directories(ChimpFramework PRIVATE "Source/api/")
target_include_directories(ChimpFramework PRIVATE "Source/impl/")

# Export all symbols
if (MSVC)
    target_compile_options(ChimpFramework PRIVATE /DCHIMP_EXPORTS /Z7 /Od)
    set_target_properties(ChimpFramework PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# Dependencies
# GLM
add_compile_definitions(CHIMP_GLM=1)
find_package(glm CONFIG REQUIRED)
target_link_libraries(ChimpFramework PUBLIC glm::glm)

# libsndfile
add_compile_definitions(CHIMP_LIBSNDFILE=1)
find_package(SndFile CONFIG REQUIRED)
target_link_libraries(ChimpFramework PUBLIC SndFile::sndfile)

# STB
add_compile_definitions(CHIMP_STB=1)
find_package(Stb REQUIRED)
target_include_directories(ChimpFramework PUBLIC ${Stb_INCLUDE_DIR})

# FLECS
add_compile_definitions(CHIMP_FLECS=1)
find_package(flecs CONFIG REQUIRED)
target_link_libraries(ChimpFramework PUBLIC $<IF:$<TARGET_EXISTS:flecs::flecs>,flecs::flecs,flecs::flecs_static>)

# ENET
add_compile_definitions(CHIMP_ENET=1)
find_package(unofficial-enet CONFIG REQUIRED)
target_link_libraries(ChimpFramework PUBLIC unofficial::enet::enet winmm)

# GLFW
add_compile_definitions(CHIMP_GLFW=1)
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(ChimpFramework PUBLIC glfw)

# OpenAL
add_compile_definitions(CHIMP_OPENAL=1)
find_package(OpenAL CONFIG REQUIRED)
target_link_libraries(ChimpFramework PUBLIC OpenAL::OpenAL)

# OpenGL
add_compile_definitions(CHIMP_OPENGL=1)
find_package(OpenGL REQUIRED)
target_link_libraries(ChimpFramework PUBLIC OpenGL::GL)
# GLEW (OpenGL Extension Wrangler Library)
find_package(GLEW REQUIRED)
target_link_libraries(ChimpFramework PUBLIC GLEW::GLEW)

## Assimp
add_compile_definitions(CHIMP_ASSIMP=1)
find_package(assimp CONFIG REQUIRED)
target_link_libraries(ChimpFramework PUBLIC assimp::assimp)

# ImGui
add_compile_definitions(CHIMP_IMGUI=1)
find_package(imgui CONFIG REQUIRED)
target_link_libraries(ChimpFramework PUBLIC imgui::imgui)

# nlohmann json
add_compile_definitions(CHIMP_NLOHMANN_JSON=1)
find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(ChimpFramework PUBLIC nlohmann_json::nlohmann_json)

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
file(REMOVE_RECURSE ${CHIMP_EXE_OUT_DIR}/ChimpData)
add_custom_command(TARGET ChimpFramework POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_CURRENT_SOURCE_DIR}/ChimpData ${CHIMP_EXE_OUT_DIR}/ChimpData
)
endif()
