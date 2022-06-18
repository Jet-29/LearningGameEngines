cmake_minimum_required(VERSION 3.22)
project(engine CXX)

include(SourceList.cmake)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

add_subdirectory(vendor/spdlog)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(vendor/GLFW)

add_subdirectory(vendor/glad)
add_subdirectory(vendor/imgui)

add_dependencies(${PROJECT_NAME} glfw spdlog glad imgui)

set(Includes
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/vendor/spdlog/include
        ${PROJECT_SOURCE_DIR}/vendor/GLFW/include
        ${PROJECT_SOURCE_DIR}/vendor/GLAD/include
        ${PROJECT_SOURCE_DIR}/vendor/imgui
        ${PROJECT_SOURCE_DIR}/vendor/glm/include
        )

set(Libraries "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/glfw3.lib ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/glad.lib ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/imgui.lib")

set(CMAKE_STATIC_LINKER_FLAGS ${Libraries})

target_include_directories(${PROJECT_NAME} PRIVATE ${Includes})
target_precompile_headers(${PROJECT_NAME} PRIVATE ${PrecompiledHeaders})