function(setup_chimp_target target_name)
    set_property(TARGET ${target_name} PROPERTY CXX_STANDARD 20)

    target_include_directories(${target_name} PRIVATE
        "Source/"
        "${CMAKE_SOURCE_DIR}/ChimpFramework/Source/"
    )

    target_link_libraries(${target_name} PRIVATE ChimpFramework)

    target_compile_definitions(${target_name} PRIVATE
        CHIMP_GLM=1
        CHIMP_LIBSNDFILE=1
        CHIMP_STB=1
        CHIMP_FLECS=1
        CHIMP_ENET=1
        CHIMP_GLFW=1
        CHIMP_OPENAL=1
        CHIMP_OPENGL=1
        CHIMP_ASSIMP=1
        CHIMP_IMGUI=1
    )

    set_game_data_macro(${target_name})
endfunction()
