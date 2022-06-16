cmake_minimum_required(VERSION 3.22)
project(sandbox CXX)

include(SourceList.cmake)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

set(Includes
        ${PROJECT_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/engine/src
        )

set(Libraries ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/engine.lib)

target_include_directories(${PROJECT_NAME} PRIVATE ${Includes})
target_link_libraries(${PROJECT_NAME} ${Libraries})