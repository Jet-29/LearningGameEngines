#pragma once

#include "engine/Window.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace Engine {

	class WindowsWindow : public Window {
	public:
		WindowsWindow(const WindowProps &props);
		~WindowsWindow() override;

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn &callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

	private:
		void Init(const WindowProps &props);
		void Shutdown();

		GLFWwindow *m_Window{};

		struct WindowData {
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};

} // Engine