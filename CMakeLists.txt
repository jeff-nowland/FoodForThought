cmake_minimum_required(VERSION 3.21)

project(f4t)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

include(Output)

find_package(Catch2 REQUIRED)


add_subdirectory(f4t)

