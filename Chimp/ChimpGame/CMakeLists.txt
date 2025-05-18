include(${CMAKE_SOURCE_DIR}/cmake/GameDataMacro.cmake)

# ChimpGame
include(ChimpGameSources.cmake)
set_property(TARGET ChimpGame PROPERTY CXX_STANDARD 20)

# Export all symbols
if (MSVC)
    target_compile_options(ChimpGame PRIVATE /DCHIMP_EXPORTS /Z7 /Od)
    set_target_properties(ChimpGame PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# Link to chimp
set_game_data_macro(ChimpGame)

target_include_directories(ChimpGame PRIVATE "Source/")
target_include_directories(ChimpGame PRIVATE "${CMAKE_SOURCE_DIR}/ChimpFramework/Source/")

target_link_libraries(ChimpGame PRIVATE Chimp)

target_compile_definitions(ChimpGame PRIVATE
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