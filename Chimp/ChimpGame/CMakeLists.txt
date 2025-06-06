include(${CMAKE_SOURCE_DIR}/cmake/GameDataMacro.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/LinkChimp.cmake)

# ChimpGame
include(ChimpGameSources.cmake)

# Export all symbols
if (MSVC)
    target_compile_options(ChimpGame PRIVATE /DCHIMP_EXPORTS /Z7 /Od)
    set_target_properties(ChimpGame PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# Link to chimp
setup_chimp_target(ChimpGame)