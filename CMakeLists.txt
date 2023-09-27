cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

project(
    Lithium
    VERSION 0.0.1
    DESCRIPTION "A programming Language"
    HOMEPAGE_URL "doesnotexist.com"
    LANGUAGES CXX)

add_executable(lithium
    src/main.cpp src/logger.cpp src/exceptions.cpp src/compiler.cpp 
    src/preprocessor.cpp src/lexer.cpp src/parser.cpp src/optimizer.cpp
    src/analyzer.cpp
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

target_include_directories(lithium PUBLIC src 
    vendor/spdlog/include
    vendor/spdlog/include/spdlog/fmt
)

set(spdlog_DIR vendor/spdlog/cmake)
find_package(spdlog REQUIRED)

set(SPDLOG_BUILD_EXAMPLE OFF CACHE BOOL "Build spdlog example" FORCE)
set(SPDLOG_BUILD_TESTS OFF CACHE BOOL "Build spdlog tests" FORCE)

target_link_libraries(lithium PRIVATE spdlog::spdlog)

add_executable(example vendor/spdlog/example/example.cpp)

target_link_libraries(example PRIVATE spdlog::spdlog)

