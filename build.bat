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
set PKG_CONFIG_PATH=C:\msys64\ucrt64\lib\pkgconfig
set PATH=C:\msys64\ucrt64\bin;%PATH%

rem Run CMake with MinGW Makefiles generator
echo Running CMake configuration...
C:\msys64\ucrt64\bin\cmake.exe -G "MinGW Makefiles" ^
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

rem Copy config file to build directory for convenience
if exist config.txt (
    copy config.txt build\config.txt >nul 2>&1
    echo Configuration file copied to build directory.
) else (
    echo Warning: config.txt not found in root directory.
)
cd ..
