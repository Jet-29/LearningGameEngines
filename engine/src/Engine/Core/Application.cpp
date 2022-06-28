#include "EnginePch.h"
#include "Application.h"

#include "Engine/Renderer/Renderer.h"

namespace Engine {

    Application* Application::s_Instance = nullptr;

    Application::Application(const std::string& name) {
        ENGINE_CORE_ASSERT(!s_Instance, "Application already exists!")
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create(Window::WindowProps(name)));
        m_Window->SetEventCallback(ENGINE_BIND_EVENT_FN(OnEvent));

        Renderer::Init();

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);

    }
    void Application::OnEvent(Event& e) {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(ENGINE_BIND_EVENT_FN(OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(ENGINE_BIND_EVENT_FN(OnWindowResize));

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

            if (!m_Minimized) {
                for (Layer* layer : m_LayerStack) {
                    layer->OnUpdate(timeStep);
                }
            }

            Engine::ImGuiLayer::Begin();
            {
                for (Layer* layer : m_LayerStack) {
                    layer->OnImGuiRender();
                }
            }
            Engine::ImGuiLayer::End();

            m_Window->OnUpdate();
        }
    }
    void Application::PushLayer(Layer* layer) {
        m_LayerStack.PushLayer(layer);
    }
    void Application::PushOverlay(Layer* overlay) {
        m_LayerStack.PushOverlay(overlay);
    }
    bool Application::OnWindowClose(WindowCloseEvent& e) {
        m_Running = false;
        return true;
    }
    bool Application::OnWindowResize(WindowResizeEvent& e) {

        if (e.GetWidth() == 0 || e.GetHeight() == 0) {
            m_Minimized = true;
            return false;
        }

        m_Minimized = false;
        Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
        return false;
    }
    void Application::Close() {
        m_Running = false;
    }
} // Engine