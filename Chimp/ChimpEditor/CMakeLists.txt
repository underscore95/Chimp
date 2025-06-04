include(${CMAKE_SOURCE_DIR}/cmake/GameDataMacro.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/CopyChimpDlls.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/LinkChimp.cmake)

# ChimpEditor
include(ChimpEditorSources.cmake)

# Link to chimp game
target_include_directories(ChimpEditor PRIVATE "${CMAKE_SOURCE_DIR}/ChimpGame/Source/")
target_link_libraries(ChimpEditor PRIVATE ChimpGame)

# Link to chimp
setup_chimp_target(ChimpEditor)

copy_chimp_dlls(ChimpEditor)

# Chimp options
target_compile_definitions(ChimpFramework PUBLIC 
       CHIMP_EDITOR=1 # We're using the editor, a bunch of minor changes like script components store the last text entered into the new script input box
       CHIMP_RESOURCE_SERIALISATION=1 # Tracks resources on mesh component etc so ECS can be serialised
)