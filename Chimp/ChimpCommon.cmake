function(setup_chimp_executable target_name)
    # Define GAME_DATA_FOLDER
    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        target_compile_definitions(${target_name} PUBLIC GAME_DATA_FOLDER="Data")
    else()
        target_compile_definitions(${target_name} PUBLIC GAME_DATA_FOLDER="${CMAKE_CURRENT_SOURCE_DIR}/Data")
    endif()

    # Copy data folder only in release
    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        add_custom_command(TARGET ${target_name} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/Data ${CMAKE_BINARY_DIR}/Data
        )
    endif()
endfunction()

function(setup_chimp_target target_name)
    target_include_directories(${target_name} PUBLIC "Source/")
    target_include_directories(${target_name} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../ChimpFramework/Source/")

    target_link_libraries(${target_name} PRIVATE Chimp)
    link_chimp_dependencies(${target_name})

    add_custom_command(TARGET ${target_name} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:Chimp>
            $<TARGET_FILE_DIR:${target_name}>
    )

    target_compile_definitions(${target_name} PUBLIC
        CHIMP_FLECS=1
        CHIMP_LIBSNDFILE=1
    )

    if (CMAKE_VERSION VERSION_GREATER 3.12)
        set_property(TARGET ${target_name} PROPERTY CXX_STANDARD 20)
    endif()
endfunction()

function(link_chimp_dependencies target_name)
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

    # GLM
    find_package(glm CONFIG REQUIRED)
    target_link_libraries(${target_name} PRIVATE glm::glm)

    # libsndfile
    find_package(SndFile CONFIG REQUIRED)
    target_link_libraries(${target_name} PRIVATE SndFile::sndfile)

    # STB (header-only)
    find_package(Stb REQUIRED)
    target_include_directories(${target_name} PRIVATE ${Stb_INCLUDE_DIR})

    # FLECS
    find_package(flecs CONFIG REQUIRED)
    target_link_libraries(${target_name} PRIVATE $<IF:$<TARGET_EXISTS:flecs::flecs>,flecs::flecs,flecs::flecs_static>)

    # ENET
    find_package(unofficial-enet CONFIG REQUIRED)
    target_link_libraries(${target_name} PRIVATE unofficial::enet::enet winmm)

    # GLFW
    find_package(glfw3 CONFIG REQUIRED)
    target_link_libraries(${target_name} PRIVATE glfw)

    # OpenAL
    find_package(OpenAL CONFIG REQUIRED)
    target_link_libraries(${target_name} PRIVATE OpenAL::OpenAL)

    # OpenGL
    find_package(OpenGL REQUIRED)
    target_link_libraries(${target_name} PRIVATE OpenGL::GL)

    # GLEW
    find_package(GLEW REQUIRED)
    target_link_libraries(${target_name} PRIVATE GLEW::GLEW)

    # Assimp
    find_package(assimp CONFIG REQUIRED)
    target_link_libraries(${target_name} PRIVATE assimp::assimp)

    # ImGui
    find_package(imgui CONFIG REQUIRED)
    target_link_libraries(${target_name} PRIVATE imgui::imgui)
endfunction()
