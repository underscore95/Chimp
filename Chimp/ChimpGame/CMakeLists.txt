include(${CMAKE_SOURCE_DIR}/ChimpCommon.cmake)

add_executable(ChimpGame "Source/GameMain.cpp" "Source/EntryScene.h" "Source/EntryScene.cpp" "Source/stdafx.h" "Source/Logger.h")

setup_chimp_target(ChimpGame)