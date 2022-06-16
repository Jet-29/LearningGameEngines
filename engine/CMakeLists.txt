cmake_minimum_required(VERSION 3.22)
project(engine CXX)

include(SourceList.cmake)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

set(Includes
        ${PROJECT_SOURCE_DIR}/src)

set(Libraries "")

set(CMAKE_STATIC_LINKER_FLAGS ${Libraries})

target_include_directories(${PROJECT_NAME} PRIVATE ${Includes})
#target_precompile_headers(${PROJECT_NAME} PRIVATE ${PrecompiledHeaders})