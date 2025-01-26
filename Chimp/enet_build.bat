@echo off

if "%1"=="" (
    echo No build type specified. Defaulting to Debug.
    set BUILD_TYPE=Debug
) else (
    set BUILD_TYPE=%1
)

cmake -S enet-src -B enet-build -DCMAKE_BUILD_TYPE=%BUILD_TYPE%
cmake --build enet-build --config %BUILD_TYPE%
