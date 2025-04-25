@echo off
echo Building SteamdbCLI with MinGW...
echo.

rem Clean up existing build directory
if exist build rmdir /s /q build
mkdir build
cd build

rem Set environment variables for the C and C++ compilers
set CMAKE_C_COMPILER=C:\msys64\ucrt64\bin\gcc.exe
set CMAKE_CXX_COMPILER=C:\msys64\ucrt64\bin\g++.exe

rem Run CMake with MinGW Makefiles generator and specify CURL paths
echo Running CMake configuration...
cmake -G "MinGW Makefiles" ^
      -DCMAKE_C_COMPILER=C:/msys64/ucrt64/bin/gcc.exe ^
      -DCMAKE_CXX_COMPILER=C:/msys64/ucrt64/bin/g++.exe ^
      -DCMAKE_PREFIX_PATH=C:/msys64/ucrt64 ^
      -DCMAKE_POLICY_DEFAULT_CMP0111=NEW ^
      ..

if %ERRORLEVEL% neq 0 (
    echo CMake configuration failed!
    cd ..
    exit /b %ERRORLEVEL%
)

echo.
echo Building with mingw32-make...
mingw32-make

if %ERRORLEVEL% neq 0 (
    echo Build failed!
    cd ..
    exit /b %ERRORLEVEL%
)

echo.
echo Build successful! The executable is located at build\SteamdbCLI.exe
cd ..