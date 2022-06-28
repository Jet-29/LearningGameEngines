cmake_minimum_required(VERSION 3.22)
project(LearningGameEngines CXX)

include(${CMAKE_SOURCE_DIR}/cmake/config.cmake)

add_subdirectory("engine")
add_subdirectory("editor")
add_subdirectory("sandbox")