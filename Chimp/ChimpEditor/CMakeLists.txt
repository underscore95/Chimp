include(${CMAKE_SOURCE_DIR}/cmake/GameDataMacro.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/CopyChimpDlls.cmake)

# ChimpEditor
include(ChimpEditorSources.cmake)
set_property(TARGET ChimpEditor PROPERTY CXX_STANDARD 20)
target_include_directories(ChimpEditor PRIVATE "Source/")

# Link to chimp game
target_include_directories(ChimpEditor PRIVATE "${CMAKE_SOURCE_DIR}/ChimpGame/Source/")
target_link_libraries(ChimpEditor PRIVATE ChimpGame)

# Link to chimp
set_game_data_macro(ChimpEditor)

target_include_directories(ChimpEditor PRIVATE "${CMAKE_SOURCE_DIR}/ChimpFramework/Source/")

target_link_libraries(ChimpEditor PRIVATE ChimpFramework)

target_compile_definitions(ChimpEditor PRIVATE
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

copy_chimp_dlls(ChimpEditor)

# Chimp options
target_compile_definitions(ChimpFramework PUBLIC 
       CHIMP_RESOURCE_SERIALISATION=1 # Tracks resources on mesh component etc so ECS can be serialised
)