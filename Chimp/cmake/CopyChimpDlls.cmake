function(copy_chimp_dlls target)
    if (WIN32)
        # Copy dlls we need
        add_custom_command (
            TARGET "${target}" POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy -t "$<TARGET_FILE_DIR:${target}>"
                    "$<TARGET_RUNTIME_DLLS:${target}>" USES_TERMINAL COMMAND_EXPAND_LISTS
        )

        # Copy some that are missed by above command
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_BINARY_DIR}/ChimpFramework/pugixml.dll"
            "${CMAKE_BINARY_DIR}/ChimpFramework/libmp3lame.DLL"
            $<TARGET_FILE_DIR:${target}>
        )

        # Copy pdb (debug symbols) for chimp
        if (CMAKE_BUILD_TYPE STREQUAL "Debug")
            add_custom_command(TARGET ${target} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${CMAKE_BINARY_DIR}/ChimpFramework/ChimpFramework.pdb"
                $<TARGET_FILE_DIR:${target}>
            )
        endif()
    endif()
endfunction()