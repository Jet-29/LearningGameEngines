#include "EnginePch.h"
#include "Application.h"

#include "Engine/Renderer/Renderer.h"

namespace Engine {

    Application *Application::s_Instance = nullptr;

    Application::Application() {
        ENGINE_CORE_ASSERT(!s_Instance, "Application already exists!")
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(ENGINE_BIND_EVENT_FN(OnEvent));

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);

        {
            m_VertexArray.reset(VertexArray::Create());

            float vertices[3 * 7] = {
                -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
            };

            uint32_t indices[3] = {0, 1, 2};

            std::shared_ptr<VertexBuffer> vertexBuffer;
            vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

            vertexBuffer->SetLayout({
                                        {ShaderDataType::Float3, "a_Position"},
                                        {ShaderDataType::Float4, "a_Color"}
                                    });

            m_VertexArray->AddVertexBuffer(vertexBuffer);

            std::shared_ptr<IndexBuffer> indexBuffer;
            indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

            m_VertexArray->SetIndexBuffer(indexBuffer);

        }

        {
            m_SquareVA.reset(VertexArray::Create());

            float squareVertices[3 * 4] = {
                -0.75f, -0.75f, 0.0f,
                0.75f, -0.75f, 0.0f,
                0.75f, 0.75f, 0.0f,
                -0.75f, 0.75f, 0.0f
            };
            uint32_t indices[6] = {0, 1, 2, 2, 3, 0};

            std::shared_ptr<VertexBuffer> squareVB;
            squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

            squareVB->SetLayout({
                                    {ShaderDataType::Float3, "a_Position"}
                                });

            m_SquareVA->AddVertexBuffer(squareVB);

            std::shared_ptr<IndexBuffer> indexBuffer;
            indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

            m_SquareVA->SetIndexBuffer(indexBuffer);
        }

        std::string vertexSrc = R"(
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

out vec4 v_Color;

void main() {
    v_Color = a_Color;
    gl_Position = vec4(a_Position, 1.0);
}

)";
        std::string fragmentSrc = R"(
#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_Color;

void main() {
    color = v_Color;
}

)";

        m_Shader = std::make_shared<Shader>(vertexSrc, fragmentSrc);

        std::string SquareVertexShader = R"(
#version 330 core

layout(location = 0) in vec3 a_Position;

void main() {
    gl_Position = vec4(a_Position, 1.0);
}

)";
        std::string SquareFragmentShader = R"(
#version 330 core

layout(location = 0) out vec4 color;

void main() {
    color = vec4(0.2, 0.3, 0.8, 1.0);
}

)";

        m_BlueShader = std::make_shared<Shader>(SquareVertexShader, SquareFragmentShader);

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

            RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
            RenderCommand::Clear();

            Renderer::BeginScene();

            m_BlueShader->Bind();
            Renderer::Submit(m_SquareVA);

            m_Shader->Bind();
            Renderer::Submit(m_VertexArray);

            Renderer::EndScene();

            for (Layer *layer : m_LayerStack) {
                layer->OnUpdate();
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