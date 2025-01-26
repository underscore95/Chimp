@echo off

REM Check if all three arguments (folder, URL, and tag) were provided
if "%~1"=="" (
    echo Usage: %0 [folder] [git-url] [tag]
    exit /b 1
)

if "%~2"=="" (
    echo Usage: %0 [folder] [git-url] [tag]
    exit /b 1
)

if "%~3"=="" (
    echo Usage: %0 [folder] [git-url] [tag]
    exit /b 1
)

REM Set arguments
set FOLDER=%~1
set URL=%~2
set TAG=%~3

REM Check if the vendor folder exists, if not create it
if not exist "vendor" (
    mkdir vendor
)

REM Change to vendor directory
cd vendor

REM Clone the repository into the specified folder
git clone %URL% %FOLDER%
cd %FOLDER% || exit /b 1

REM Checkout the specified tag
git checkout %TAG%
exit /b 0
