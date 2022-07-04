cmake_minimum_required(VERSION 3.22)
project(engine CXX)

include(SourceList.cmake)

add_library(${PROJECT_NAME} STATIC ${ALL_SOURCE_FILES})

add_subdirectory(vendor/spdlog)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(vendor/GLFW)

add_subdirectory(vendor/glad)
add_subdirectory(vendor/glm)

set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "" FORCE)
add_subdirectory(vendor/yaml-cpp)
add_compile_definitions(YAML_CPP_STATIC_DEFINE)

add_dependencies(${PROJECT_NAME} glfw spdlog glad yaml-cpp)

set(Includes
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/vendor/spdlog/include
        ${PROJECT_SOURCE_DIR}/vendor/GLFW/include
        ${PROJECT_SOURCE_DIR}/vendor/GLAD/include
        ${PROJECT_SOURCE_DIR}/vendor/imgui
        ${PROJECT_SOURCE_DIR}/vendor/glm
        ${PROJECT_SOURCE_DIR}/vendor/stb_image
        ${PROJECT_SOURCE_DIR}/vendor/entt
        ${PROJECT_SOURCE_DIR}/vendor/yaml-cpp/include
        ${PROJECT_SOURCE_DIR}/vendor/ImGuizmo
        )

set(Libraries "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/glfw3.lib ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/glad.lib ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/yaml-cpp${DEBUG_SUFFIX}.lib")

set(CMAKE_STATIC_LINKER_FLAGS ${Libraries})

target_include_directories(${PROJECT_NAME} PRIVATE ${Includes})
target_precompile_headers(${PROJECT_NAME} PRIVATE ${PrecompiledHeaders})