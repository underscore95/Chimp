cmake_minimum_required(VERSION 3.12)

set(CHIMP_OUT_DIR ${CMAKE_BINARY_DIR}/Chimp)

add_subdirectory("ChimpFramework")
add_subdirectory("ChimpGame")
add_subdirectory("ChimpEditor")