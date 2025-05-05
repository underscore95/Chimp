include(${CMAKE_SOURCE_DIR}/ChimpCommon.cmake)

add_executable(ChimpEditor "Source/Main.cpp" "Source/Logger.h" "Source/stdafx.h" "Source/EditorScene.h" "Source/EditorScene.cpp")

setup_chimp_target(ChimpEditor)