cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("ChimpGame")

add_library (ChimpGame STATIC "src/GameMain.cpp" "src/menu/MenuScene.h" "src/menu/MenuScene.cpp" "src/stdafx.h" "src/Logger.h")

# Add src as include dir
target_include_directories(ChimpGame PUBLIC "src/")

# Include chimp
add_compile_definitions(CHIMP_FLECS=1)
add_compile_definitions(CHIMP_LIBSNDFILE=1)
target_include_directories(ChimpGame PUBLIC "../src/")

  find_package(flecs CONFIG REQUIRED)
  find_package(imgui CONFIG REQUIRED)
target_link_libraries(ChimpGame PRIVATE imgui flecs::flecs_static)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ChimpGame PROPERTY CXX_STANDARD 20)
endif()

# In debug mode, we'll just use the data folder in the source directory
# in release mode, we'll copy it to the build directory and use that one
if (CMAKE_BUILD_TYPE STREQUAL "Release")
add_compile_definitions(GAME_DATA_FOLDER="Data")
else()
add_compile_definitions(GAME_DATA_FOLDER="${CMAKE_CURRENT_SOURCE_DIR}/Data")
endif()

# Copy data folder into build directory
# only copy in release
if (CMAKE_BUILD_TYPE STREQUAL "Release")
file(REMOVE_RECURSE ${CMAKE_BINARY_DIR}/Data)
add_custom_command(TARGET ChimpGame POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_CURRENT_SOURCE_DIR}/Data ${CMAKE_BINARY_DIR}/Data
)
endif()