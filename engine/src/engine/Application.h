#pragma once

#include "Base.h"
#include "Window.h"
#include "engine/Events/ApplicationEvent.h"
#include "engine/LayerStack.h"
#include "engine/ImGui/ImGuiLayer.h"

namespace Engine {
	class ENGINE_API Application {
	public:
		Application();
		virtual ~Application() = default;

		void Run();
		void OnEvent(Event &e);

		void PushLayer(Layer *layer);
		void PushOverlay(Layer *overlay);

		inline Window &GetWindow() { return *m_Window; }

		inline static Application &Get() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent &e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;

		static Application *s_Instance;
	};

	// to define in client
	Application *CreateApplication();

} // Engine