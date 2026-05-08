cmake_minimum_required(VERSION 3.15)

project(zol VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES src/*.cxx)

include_directories(include)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

add_subdirectory(tests)