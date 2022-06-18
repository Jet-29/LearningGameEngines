set(SOURCE_FILES
        "src/EnginePch.cpp"
        "src/Engine/Core/Application.cpp"
        "src/Engine/Core/Log.cpp"
        "src/Platforms/Windows/WindowsWindow.cpp"
        "src/Engine/Core/Layer.cpp"
        "src/Engine/Core/LayerStack.cpp"
        "src/Engine/ImGui/ImGuiLayer.cpp"
        "src/Platforms/OpenGL/ImGuiOpenGLRenderer.cpp"
        "src/Platforms/GLFW/ImGuiGLFWRenderer.cpp"
        "src/Platforms/Windows/WindowsInput.cpp"
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