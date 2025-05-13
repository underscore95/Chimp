include(${CMAKE_SOURCE_DIR}/ChimpCommon.cmake)

add_library(ChimpGame SHARED "Source/stdafx.h" "Source/Logger.h" "Source/TestClass.h" "Source/TestClass.cpp")

# Export all symbols
if (MSVC)
    target_compile_options(ChimpGame PRIVATE /DCHIMP_EXPORTS /Z7 /Od)
    set_target_properties(ChimpGame PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

setup_chimp_target(ChimpGame)