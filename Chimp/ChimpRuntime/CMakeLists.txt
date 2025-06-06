include(${CMAKE_SOURCE_DIR}/cmake/GameDataMacro.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/CopyChimpDlls.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/LinkChimp.cmake)

# ChimpRuntime
include(ChimpRuntimeSources.cmake)

# Link to chimp game
target_include_directories(ChimpRuntime PRIVATE "${CMAKE_SOURCE_DIR}/ChimpGame/Source/")
target_link_libraries(ChimpRuntime PRIVATE ChimpGame)

# Link to chimp
setup_chimp_target(ChimpRuntime)

copy_chimp_dlls(ChimpRuntime)