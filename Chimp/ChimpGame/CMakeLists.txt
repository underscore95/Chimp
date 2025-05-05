include(${CMAKE_SOURCE_DIR}/ChimpCommon.cmake)

add_executable(ChimpGame "Source/GameMain.cpp" "Source/EntryScene.h" "Source/EntryScene.cpp" "Source/stdafx.h" "Source/Logger.h")

target_include_directories(ChimpGame PUBLIC "Source/")
target_link_libraries(ChimpGame PRIVATE Chimp)

add_custom_command(TARGET ChimpGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:Chimp>
    $<TARGET_FILE_DIR:ChimpGame>
)

setup_chimp_target(ChimpGame)

# Copy data folder in release
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    file(REMOVE_RECURSE ${CMAKE_BINARY_DIR}/Data)
    add_custom_command(TARGET ChimpGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/Data ${CMAKE_BINARY_DIR}/Data
    )
endif()
