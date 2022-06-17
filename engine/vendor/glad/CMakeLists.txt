cmake_minimum_required(VERSION 3.22)
project(glad C)

add_library(${PROJECT_NAME} STATIC ${PROJECT_SOURCE_DIR}/src/glad.c)

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)
