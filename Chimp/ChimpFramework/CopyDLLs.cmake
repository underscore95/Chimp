# DLLs that are the same in both Release and Debug
set(COMMON_DLLS
    "OpenAL32.dll"
    "ogg.dll"
    "opus.dll"
    "poly2tri.dll"
    "pugixml.dll"
    "sndfile.dll"
    "vorbis.dll"
    "vorbisenc.dll"
)

set(RELEASE_DLLS
    "assimp-vc143-mt.dll"
    "brotlicommon.dll"
    "brotlidec.dll"
    "bz2.dll"
    "FLAC.dll"
    "freetype.dll"
    "glew32.dll"
    "glfw3.dll"
    "libmp3lame.DLL"
    "libpng16.dll"
    "minizip.dll"
    "mpg123.dll"
    "zlib1.dll"
)

set(DEBUG_DLLS
    "assimp-vc143-mtd.dll"
    "brotlicommon.dll"
    "brotlidec.dll"
    "bz2d.dll"
    "FLAC.dll"
    "freetyped.dll"
    "glew32d.dll"
    "glfw3.dll"
    "libmp3lame.DLL"
    "libpng16d.dll"
    "minizip.dll"
    "mpg123.dll"
    "zlibd1.dll"
)

# Function to copy a single DLL file
function(copy_dll FILE)
    add_custom_command(TARGET Chimp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_BINARY_DIR}/${FILE}"
            "${CHIMP_EXE_OUT_DIR}/${FILE}"
        COMMENT "Copying ${FILE} to ${CHIMP_EXE_OUT_DIR}"
    )
endfunction()

# Copy dlls
foreach(DLL ${COMMON_DLLS})
    copy_dll(${DLL})
endforeach()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    foreach(DLL ${RELEASE_DLLS})
        copy_dll(${DLL})
    endforeach()
else()
    foreach(DLL ${DEBUG_DLLS})
        copy_dll(${DLL})
    endforeach()
endif()
