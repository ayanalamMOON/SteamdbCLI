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
    src/steam_api_helper.cpp
)

# Find and link external libraries
find_package(PkgConfig REQUIRED)
pkg_check_modules(CURL REQUIRED libcurl)

target_include_directories(SteamdbCLI PRIVATE ${CURL_INCLUDE_DIRS})
target_link_libraries(SteamdbCLI ${CURL_LIBRARIES})
target_compile_options(SteamdbCLI PRIVATE ${CURL_CFLAGS_OTHER})

# Add threads support
find_package(Threads REQUIRED)
target_link_libraries(SteamdbCLI Threads::Threads)

# Add Windows-specific libraries
target_link_libraries(SteamdbCLI ws2_32 wldap32 crypt32)
