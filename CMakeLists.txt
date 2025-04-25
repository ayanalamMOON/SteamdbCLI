cmake_minimum_required(VERSION 3.10)

# Set policy CMP0111 to NEW to avoid IMPORTED_LOCATION warnings
if(POLICY CMP0111)
  cmake_policy(SET CMP0111 NEW)
endif()

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
    src/error_handling.cpp
)

# Find and link external libraries
# For CURL, manually specify package components to avoid issues
find_package(CURL REQUIRED)
if(CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIRS})
    target_link_libraries(SteamdbCLI ${CURL_LIBRARIES})
else()
    message(FATAL_ERROR "CURL library not found. Please install libcurl development files.")
endif()

# Add threads support
find_package(Threads REQUIRED)
target_link_libraries(SteamdbCLI ${CMAKE_THREAD_LIBS_INIT})

# Print status messages
message(STATUS "CURL include dir: ${CURL_INCLUDE_DIRS}")
message(STATUS "CURL libraries: ${CURL_LIBRARIES}")
