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
        "src/Engine/Renderer/Renderer2D.cpp"
        "src/Engine/Renderer/Shader.cpp"
        "src/Engine/Renderer/Buffer.cpp"
        "src/Engine/Renderer/VertexArray.cpp"
        "src/Engine/Renderer/OrthographicCamera.cpp"
        "src/Engine/Renderer/OrthographicCameraController.cpp"
        "src/Engine/Renderer/Texture.cpp"
        "src/Engine/Renderer/SubTexture2D.cpp"
        "src/Engine/Renderer/FrameBuffer.cpp"
        "src/Engine/Scene/Scene.cpp"
        "src/Engine/Scene/Components.cpp"
        "src/Engine/Scene/Entity.cpp"
        "src/Engine/Scene/ScriptableEntity.cpp"
        "src/Engine/Scene/SceneCamera.cpp"
        )

Set(OPENGL_SOURCE_FILES
        "src/Platforms/OpenGL/OpenGLContext.cpp"
        "src/Platforms/OpenGL/OpenGLRendererAPI.cpp"
        "src/Platforms/OpenGL/OpenGLBuffer.cpp"
        "src/Platforms/OpenGL/OpenGLVertexArray.cpp"
        "src/Platforms/OpenGL/OpenGLShader.cpp"
        "src/Platforms/OpenGL/OpenGLTexture.cpp"
        "src/Platforms/OpenGL/OpenGLFrameBuffer.cpp"
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

set(STB_SOURCE_FILES
        "vendor/stb_image/stb_image.cpp"
        )

Set(ALL_SOURCE_FILES
        ${ENGINE_SOURCE_FILES}
        ${PLATFORM_SOURCE}
        ${IMGUI_SOURCE_FILES}
        ${STB_SOURCE_FILES}
        )