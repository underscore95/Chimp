function(set_game_data_macro target_name)
    # Define GAME_DATA_FOLDER
    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        target_compile_definitions(${target_name} PUBLIC GAME_DATA_FOLDER="Data")
    else()
        target_compile_definitions(${target_name} PUBLIC GAME_DATA_FOLDER="${CMAKE_CURRENT_SOURCE_DIR}/Data")
    endif()

    # Copy data folder only in release
    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/Data)
        add_custom_command(TARGET ${target_name} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/Data ${CMAKE_CURRENT_BINARY_DIR}/Data
        )
    endif()
endfunction()