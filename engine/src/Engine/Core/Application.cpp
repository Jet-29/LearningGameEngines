#include "EnginePch.h"
#include "Application.h"

namespace Engine {

    Application *Application::s_Instance = nullptr;

    Application::Application() {
        ENGINE_CORE_ASSERT(!s_Instance, "Application already exists!")
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(ENGINE_BIND_EVENT_FN(OnEvent));

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);

    }
    void Application::OnEvent(Event &e) {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(ENGINE_BIND_EVENT_FN(OnWindowClose));

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
            (*--it)->OnEvent(e);
            if (e.Handled) {
                break;
            }
        }
    }
    void Application::Run() {
        while (m_Running) {
            float time = m_Window->GetTime();
            TimeStep timeStep = time - m_LastFrameTime;
            m_LastFrameTime = time;

            for (Layer *layer : m_LayerStack) {
                layer->OnUpdate(timeStep);
            }

            Engine::ImGuiLayer::Begin();
            for (Layer *layer : m_LayerStack) {
                layer->OnImGuiRender();
            }
            Engine::ImGuiLayer::End();

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