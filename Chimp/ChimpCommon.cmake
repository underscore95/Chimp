function(setup_chimp_target target_name)
    target_compile_definitions(${target_name} PUBLIC
        CHIMP_FLECS=1
        CHIMP_LIBSNDFILE=1
    )

    target_include_directories(${target_name} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/../ChimpFramework/Source/"
    )

    find_package(flecs CONFIG REQUIRED)
    find_package(imgui CONFIG REQUIRED)

    target_link_libraries(${target_name} PRIVATE imgui flecs::flecs_static)

    if (CMAKE_VERSION VERSION_GREATER 3.12)
        set_property(TARGET ${target_name} PROPERTY CXX_STANDARD 20)
    endif()

    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        target_compile_definitions(${target_name} PUBLIC GAME_DATA_FOLDER="Data")
    else()
        target_compile_definitions(${target_name} PUBLIC GAME_DATA_FOLDER="${CMAKE_CURRENT_SOURCE_DIR}/Data")
    endif()
endfunction()
