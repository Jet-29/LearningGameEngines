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

set(SHADERC_SKIP_TESTS ON)
set(SHADERC_SKIP_EXAMPLES ON)
set(SHADERC_SKIP_COPYRIGHT_CHECK ON)
set(SKIP_SPIRV_TOOLS_INSTALL ON)
set(SPIRV_HEADERS_SKIP_EXAMPLES ON)
set(SKIP_GLSLANG_INSTALL ON)
add_subdirectory(vendor/shaderc)

add_subdirectory(vendor/SPIRV-Cross)

set(BOX2D_BUILD_UNIT_TESTS OFF)
set(BOX2D_BUILD_TESTBED OFF)
add_subdirectory(vendor/Box2D)

#set(VULKAN_DIR "C:/VulkanSDK/1.3.216.0")
#find_package(Vulkan REQUIRED)

add_dependencies(${PROJECT_NAME} glfw spdlog glad yaml-cpp shaderc spirv-cross-core spirv-cross-glsl box2d)

set(Includes
        ${PROJECT_SOURCE_DIR}/src
        #        ${Vulkan_INCLUDE_DIR}
        ${PROJECT_SOURCE_DIR}/vendor/spdlog/include
        ${PROJECT_SOURCE_DIR}/vendor/GLFW/include
        ${PROJECT_SOURCE_DIR}/vendor/GLAD/include
        ${PROJECT_SOURCE_DIR}/vendor/imgui
        ${PROJECT_SOURCE_DIR}/vendor/glm
        ${PROJECT_SOURCE_DIR}/vendor/stb_image
        ${PROJECT_SOURCE_DIR}/vendor/entt
        ${PROJECT_SOURCE_DIR}/vendor/yaml-cpp/include
        ${PROJECT_SOURCE_DIR}/vendor/ImGuizmo
        ${PROJECT_SOURCE_DIR}/vendor/shaderc/libshaderc/include
        ${PROJECT_SOURCE_DIR}/vendor/SPIRV-Cross
        ${PROJECT_SOURCE_DIR}/vendor/Box2D/include
        )

set(Libraries "${Libraries} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/glfw3.lib")
set(Libraries "${Libraries} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/glad.lib")
set(Libraries "${Libraries} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/yaml-cpp${DEBUG_SUFFIX}.lib")
set(Libraries "${Libraries} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaderc.lib")
set(Libraries "${Libraries} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaderc_util.lib")
set(Libraries "${Libraries} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/spirv-cross-core${DEBUG_SUFFIX}.lib")
set(Libraries "${Libraries} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/spirv-cross-glsl${DEBUG_SUFFIX}.lib")
set(Libraries "${Libraries} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/OGLCompiler${DEBUG_SUFFIX}.lib")
set(Libraries "${Libraries} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/GenericCodeGen${DEBUG_SUFFIX}.lib")
set(Libraries "${Libraries} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/glslang${DEBUG_SUFFIX}.lib")
set(Libraries "${Libraries} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/HLSL${DEBUG_SUFFIX}.lib")
set(Libraries "${Libraries} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/MachineIndependent${DEBUG_SUFFIX}.lib")
set(Libraries "${Libraries} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/OSDependent${DEBUG_SUFFIX}.lib")
set(Libraries "${Libraries} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/SPIRV-Tools.lib")
set(Libraries "${Libraries} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/SPIRV-Tools-opt.lib")
set(Libraries "${Libraries} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/SPIRV${DEBUG_SUFFIX}.lib")
set(Libraries "${Libraries} ${CMAKE_CURRENT_BINARY_DIR}/vendor/Box2D/bin/box2d.lib")

set(CMAKE_STATIC_LINKER_FLAGS ${Libraries})

target_include_directories(${PROJECT_NAME} PRIVATE ${Includes})
target_precompile_headers(${PROJECT_NAME} PRIVATE ${PrecompiledHeaders})