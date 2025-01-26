@echo off

if "%1"=="" (
    echo No build type specified. Defaulting to Debug.
    set BUILD_TYPE=Debug
) else (
    set BUILD_TYPE=%1
)

cmake -S libsndfile-src -B libsndfile-build -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DBUILD_EXAMPLES=OFF -DBUILD_TESTING=OFF
cmake --build libsndfile-build --config %BUILD_TYPE%
