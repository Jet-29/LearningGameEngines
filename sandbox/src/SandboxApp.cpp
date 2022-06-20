#include <Engine.h>

class ExampleLayer : public Engine::Layer {
public:
    ExampleLayer() : Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f) {}
    virtual void OnAttach() override {
        {
            m_VertexArray.reset(Engine::VertexArray::Create());

            float vertices[3 * 7] = {
                -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
            };

            uint32_t indices[3] = {0, 1, 2};

            std::shared_ptr<Engine::VertexBuffer> vertexBuffer;
            vertexBuffer.reset(Engine::VertexBuffer::Create(vertices, sizeof(vertices)));

            vertexBuffer->SetLayout({
                                        {Engine::ShaderDataType::Float3, "a_Position"},
                                        {Engine::ShaderDataType::Float4, "a_Color"}
                                    });

            m_VertexArray->AddVertexBuffer(vertexBuffer);

            std::shared_ptr<Engine::IndexBuffer> indexBuffer;
            indexBuffer.reset(Engine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

            m_VertexArray->SetIndexBuffer(indexBuffer);

        }

        {
            m_SquareVA.reset(Engine::VertexArray::Create());

            float squareVertices[3 * 4] = {
                -0.75f, -0.75f, 0.0f,
                0.75f, -0.75f, 0.0f,
                0.75f, 0.75f, 0.0f,
                -0.75f, 0.75f, 0.0f
            };
            uint32_t indices[6] = {0, 1, 2, 2, 3, 0};

            std::shared_ptr<Engine::VertexBuffer> squareVB;
            squareVB.reset(Engine::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

            squareVB->SetLayout({
                                    {Engine::ShaderDataType::Float3, "a_Position"}
                                });

            m_SquareVA->AddVertexBuffer(squareVB);

            std::shared_ptr<Engine::IndexBuffer> indexBuffer;
            indexBuffer.reset(Engine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

            m_SquareVA->SetIndexBuffer(indexBuffer);
        }

        std::string vertexSrc = R"(
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

uniform mat4 u_ViewProjection;

out vec4 v_Color;

void main() {
    v_Color = a_Color;
    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
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

        m_Shader = std::make_shared<Engine::Shader>(vertexSrc, fragmentSrc);

        std::string SquareVertexShader = R"(
#version 330 core

layout(location = 0) in vec3 a_Position;

uniform mat4 u_ViewProjection;


void main() {
    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

)";
        std::string SquareFragmentShader = R"(
#version 330 core

layout(location = 0) out vec4 color;

void main() {
    color = vec4(0.2, 0.3, 0.8, 1.0);
}

)";

        m_BlueShader = std::make_shared<Engine::Shader>(SquareVertexShader, SquareFragmentShader);

    }

    virtual void OnUpdate() override {

        if (Engine::Input::IsKeyPressed(Engine::Key::Left)) {
            m_CameraPosition.x -= m_CameraMoveSpeed;
        }
        if (Engine::Input::IsKeyPressed(Engine::Key::Right)) {
            m_CameraPosition.x += m_CameraMoveSpeed;
        }
        if (Engine::Input::IsKeyPressed(Engine::Key::Up)) {
            m_CameraPosition.y += m_CameraMoveSpeed;
        }
        if (Engine::Input::IsKeyPressed(Engine::Key::Down)) {
            m_CameraPosition.y -= m_CameraMoveSpeed;
        }
        if (Engine::Input::IsKeyPressed(Engine::Key::A)) {
            m_CameraRotation += m_CameraRotationSpeed;
        }
        if (Engine::Input::IsKeyPressed(Engine::Key::D)) {
            m_CameraRotation -= m_CameraRotationSpeed;
        }
        m_Camera.SetPosition(m_CameraPosition);
        m_Camera.SetRotation(m_CameraRotation);

        Engine::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        Engine::RenderCommand::Clear();
        Engine::Renderer::BeginScene(m_Camera);
        Engine::Renderer::Submit(m_BlueShader, m_SquareVA);
        Engine::Renderer::Submit(m_Shader, m_VertexArray);
        Engine::Renderer::EndScene();
    }

private:
    std::shared_ptr<Engine::Shader> m_Shader;
    std::shared_ptr<Engine::Shader> m_BlueShader;

    std::shared_ptr<Engine::VertexArray> m_VertexArray;
    std::shared_ptr<Engine::VertexArray> m_SquareVA;

    Engine::OrthographicCamera m_Camera;
    glm::vec3 m_CameraPosition{0.0f};
    float m_CameraRotation = 0.0f;

    float m_CameraMoveSpeed = 0.1f;
    float m_CameraRotationSpeed = 2.0f;
};

class Sandbox : public Engine::Application {
public:
    Sandbox() {
        PushLayer(new ExampleLayer());
    }

    ~Sandbox() {

    }

};

Engine::Application *Engine::CreateApplication() {
    return new Sandbox();
}