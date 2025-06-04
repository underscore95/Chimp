function(set_game_data_macro target_name)
    if (target_name STREQUAL "ChimpGame")
        # We are ChimpGame so need to manage the data directory
        # Set the macro
        if (CMAKE_BUILD_TYPE STREQUAL "Release")
            target_compile_definitions(${target_name} PUBLIC GAME_DATA_FOLDER="Data")
        else()
            target_compile_definitions(${target_name} PUBLIC GAME_DATA_FOLDER="${CMAKE_CURRENT_SOURCE_DIR}/Data")
        endif()

        # Copy the directory in release mode
        if (CMAKE_BUILD_TYPE STREQUAL "Release")
            file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/Data)
            add_custom_command(TARGET ${target_name} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CMAKE_CURRENT_SOURCE_DIR}/Data ${CMAKE_CURRENT_BINARY_DIR}/Data
            )
        endif()
    else()
        # We aren't ChimpGamem, define the macro to point to ChimpGame's data folder
        if (CMAKE_BUILD_TYPE STREQUAL "Release")
            target_compile_definitions(${target_name} PUBLIC GAME_DATA_FOLDER="../ChimpGame/Data")
        else()
            target_compile_definitions(${target_name} PUBLIC GAME_DATA_FOLDER="${CMAKE_SOURCE_DIR}/ChimpGame/Data")
        endif()
    endif()
endfunction()
