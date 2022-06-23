set(ENGINE_SOURCE_FILES
        "src/EnginePch.cpp"
        "src/Engine/Core/Application.cpp"
        "src/Engine/Core/Log.cpp"
        "src/Engine/Core/Layer.cpp"
        "src/Engine/Core/LayerStack.cpp"
        "src/Engine/ImGui/ImGuiLayer.cpp"
        "src/Engine/ImGui/ImGuiBuild.cpp"
        "src/Engine/Renderer/Renderer.cpp"
        "src/Engine/Renderer/RendererAPI.cpp"
        "src/Engine/Renderer/RenderCommand.cpp"
        "src/Engine/Renderer/Shader.cpp"
        "src/Engine/Renderer/Buffer.cpp"
        "src/Engine/Renderer/VertexArray.cpp"
        "src/Engine/Renderer/OrthographicCamera.cpp"
        )

Set(OPENGL_SOURCE_FILES
        "src/Platforms/OpenGL/OpenGLContext.cpp"
        "src/Platforms/OpenGL/OpenGLRendererAPI.cpp"
        "src/Platforms/OpenGL/OpenGLBuffer.cpp"
        "src/Platforms/OpenGL/OpenGLVertexArray.cpp"
        "src/Platforms/OpenGL/OpenGLShader.cpp"
        )

Set(WINDOWS_SOURCE_FILES
        "src/Platforms/Windows/WindowsWindow.cpp"
        "src/Platforms/Windows/WindowsInput.cpp"
        )

Set(PLATFORM_SOURCE
        ${OPENGL_SOURCE_FILES}
        ${WINDOWS_SOURCE_FILES}
        )

set(PrecompiledHeaders
        "src/EnginePch.h"
        )

set(IMGUI_SOURCE_FILES
        "vendor/imgui/imgui.cpp"
        "vendor/imgui/imgui_draw.cpp"
        "vendor/imgui/imgui_widgets.cpp"
        "vendor/imgui/imgui_tables.cpp"
        "vendor/imgui/imgui_demo.cpp"
        )

Set(ALL_SOURCE_FILES
        ${ENGINE_SOURCE_FILES}
        ${PLATFORM_SOURCE}
        ${IMGUI_SOURCE_FILES}
        )