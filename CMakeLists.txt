cmake_minimum_required(VERSION 3.10)

# Project name and version
project(SteamdbCLI VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include header files
include_directories(include)

# Add source files
add_executable(SteamdbCLI
    src/main.cpp
    src/scraper.cpp
    src/network_utils.cpp
    src/rate_limiter.cpp
    src/logger.cpp
    src/game_data.cpp
    src/config.cpp
    src/cli_arguments.cpp
    src/game_cache.cpp
)

# Find and link external libraries
find_package(CURL REQUIRED)
target_link_libraries(SteamdbCLI CURL::libcurl)

# Add threads support if needed
find_package(Threads REQUIRED)
target_link_libraries(SteamdbCLI ${CMAKE_THREAD_LIBS_INIT})
