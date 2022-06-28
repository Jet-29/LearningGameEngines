cmake_minimum_required(VERSION 3.22)
project(editor CXX)

include(SourceList.cmake)

add_executable(${PROJECT_NAME} ${ALL_SOURCE_FILES})

add_dependencies(${PROJECT_NAME} engine)


set(Includes
        ${PROJECT_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/engine/src
        ${CMAKE_SOURCE_DIR}/engine/vendor/spdlog/include
        ${CMAKE_SOURCE_DIR}/engine/vendor/imgui
        ${CMAKE_SOURCE_DIR}/engine/vendor/glm
        )


set(Libraries
        ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/engine.lib
        )

target_include_directories(${PROJECT_NAME} PRIVATE ${Includes})
target_link_libraries(${PROJECT_NAME} ${Libraries})