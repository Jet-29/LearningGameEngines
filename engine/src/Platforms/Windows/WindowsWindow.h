#pragma once

#include "Engine/Core/Window.h"
#include "Engine/Renderer/GraphicsContext.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace Engine {

    class WindowsWindow : public Window {
    public:
        WindowsWindow(const WindowProps& props);
        virtual ~WindowsWindow() override;

        virtual void OnUpdate() override;

        virtual inline uint32_t GetWidth() const override { return m_Data.Width; }
        virtual inline uint32_t GetHeight() const override { return m_Data.Height; }

        // Window attributes
        virtual inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
        virtual void SetVSync(bool enabled) override;
        virtual bool IsVSync() const override;
        virtual float GetTime() const override;

        virtual void* GetNativeWindow() const override { return m_Window; }

    private:
        void Init(const WindowProps& props);
        void Shutdown();

        GLFWwindow* m_Window{};
        GraphicsContext* m_Context;

        struct WindowData {
            std::string Title;
            uint32_t Width, Height;
            bool VSync;

            EventCallbackFn EventCallback;
        };

        WindowData m_Data;
    };

} // Engine