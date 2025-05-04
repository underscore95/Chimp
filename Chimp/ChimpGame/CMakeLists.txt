# Include shared config
include(${CMAKE_SOURCE_DIR}/ChimpCommon.cmake)

add_library(ChimpGame STATIC 
    "src/GameMain.cpp"
    "src/EntryScene.h"
    "src/EntryScene.cpp"
    "src/stdafx.h"
    "src/Logger.h"
)

target_include_directories(ChimpGame PUBLIC "src/")
setup_chimp_target(ChimpGame)

# Copy data folder in release
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    file(REMOVE_RECURSE ${CMAKE_BINARY_DIR}/Data)
    add_custom_command(TARGET ChimpGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/Data ${CMAKE_BINARY_DIR}/Data
    )
endif()
