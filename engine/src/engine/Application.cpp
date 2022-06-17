#include "Application.h"

#include "engine/Log.h"

namespace Engine {

	Application *Application::s_Instance = nullptr;

	Application::Application() {
		ENGINE_CORE_ASSERT(!s_Instance, "Application already exists!")
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(ENGINE_BIND_EVENT_FN(OnEvent));
	}
	void Application::OnEvent(Event &e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(ENGINE_BIND_EVENT_FN(OnWindowClose));

		ENGINE_CORE_TRACE("{0}", e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.Handled) {
				break;
			}
		}
	}
	void Application::Run() {
		while (m_Running) {

			for (Layer *layer : m_LayerStack) {
				layer->OnUpdate();
			}

			m_Window->OnUpdate();
		}
	}
	bool Application::OnWindowClose(WindowCloseEvent &e) {
		m_Running = false;
		return true;
	}
	void Application::PushLayer(Layer *layer) {
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}
	void Application::PushOverlay(Layer *overlay) {
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}
} // Engine