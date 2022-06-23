#include <Engine.h>
#include "Platforms/OpenGL/OpenGLShader.h"
#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"

class ExampleLayer : public Engine::Layer {
public:
    ExampleLayer() : Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f) {}
    virtual void OnAttach() override {
        {
            m_VertexArray = Engine::VertexArray::Create();

            float vertices[3 * 7] = {
                -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
            };

            uint32_t indices[3] = {0, 1, 2};

            Engine::Ref<Engine::VertexBuffer> vertexBuffer = Engine::VertexBuffer::Create(vertices, sizeof(vertices));

            vertexBuffer->SetLayout({
                                        {Engine::ShaderDataType::Float3, "a_Position"},
                                        {Engine::ShaderDataType::Float4, "a_Color"}
                                    });

            m_VertexArray->AddVertexBuffer(vertexBuffer);

            Engine::Ref<Engine::IndexBuffer> indexBuffer = Engine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

            m_VertexArray->SetIndexBuffer(indexBuffer);

        }

        {
            m_SquareVA = Engine::VertexArray::Create();

            float squareVertices[5 * 4] = {
                -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
                0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
                0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
                -0.5f, 0.5f, 0.0f, 0.0f, 1.0f
            };
            uint32_t indices[6] = {0, 1, 2, 2, 3, 0};

            Engine::Ref<Engine::VertexBuffer> squareVB = Engine::VertexBuffer::Create(squareVertices, sizeof(squareVertices));

            squareVB->SetLayout({
                                    {Engine::ShaderDataType::Float3, "a_Position"},
                                    {Engine::ShaderDataType::Float2, "a_TexCoord"}
                                });

            m_SquareVA->AddVertexBuffer(squareVB);

            Engine::Ref<Engine::IndexBuffer> indexBuffer = Engine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

            m_SquareVA->SetIndexBuffer(indexBuffer);
        }

        std::string vertexSrc = R"(
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec4 v_Color;

void main() {
    v_Color = a_Color;
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
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

        m_Shader = Engine::Shader::Create(vertexSrc, fragmentSrc);

        std::string flatColorShaderVertexSrc = R"(
#version 330 core

layout(location = 0) in vec3 a_Position;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;


void main() {
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

)";
        std::string flatColorShaderFragmentShader = R"(
#version 330 core

layout(location = 0) out vec4 color;

uniform vec3 u_Color;

void main() {
    color = vec4(u_Color, 1.0);
}

)";

        m_FlatShader = Engine::Shader::Create(flatColorShaderVertexSrc, flatColorShaderFragmentShader);

        std::string TextureShaderVertexSrc = R"(
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec2 v_TexCoord;

void main() {
    v_TexCoord = a_TexCoord;
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

)";
        std::string TextureShaderFragmentShader = R"(
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform vec3 u_Color;
uniform sampler2D u_Texture;

void main() {
    color = texture(u_Texture, v_TexCoord) * vec4(u_Color, 1.0);
}

)";

        m_TextureShader = Engine::Shader::Create(TextureShaderVertexSrc, TextureShaderFragmentShader);

        m_Texture = Engine::Texture2D::Create("assets/textures/Checkerboard.png");
        std::dynamic_pointer_cast<Engine::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);
    }

    virtual void OnUpdate(Engine::TimeStep ts) override {

        if (Engine::Input::IsKeyPressed(Engine::Key::Left)) {
            m_CameraPosition.x -= m_CameraMoveSpeed * ts;
        }
        if (Engine::Input::IsKeyPressed(Engine::Key::Right)) {
            m_CameraPosition.x += m_CameraMoveSpeed * ts;
        }
        if (Engine::Input::IsKeyPressed(Engine::Key::Up)) {
            m_CameraPosition.y += m_CameraMoveSpeed * ts;
        }
        if (Engine::Input::IsKeyPressed(Engine::Key::Down)) {
            m_CameraPosition.y -= m_CameraMoveSpeed * ts;
        }
        if (Engine::Input::IsKeyPressed(Engine::Key::A)) {
            m_CameraRotation += m_CameraRotationSpeed * ts;
        }
        if (Engine::Input::IsKeyPressed(Engine::Key::D)) {
            m_CameraRotation -= m_CameraRotationSpeed * ts;
        }
        if (Engine::Input::IsKeyPressed(Engine::Key::J)) {
            m_SquarePosition.x -= m_SquareMoveSpeed * ts;
        }
        if (Engine::Input::IsKeyPressed(Engine::Key::L)) {
            m_SquarePosition.x += m_SquareMoveSpeed * ts;
        }
        if (Engine::Input::IsKeyPressed(Engine::Key::I)) {
            m_SquarePosition.y += m_SquareMoveSpeed * ts;
        }
        if (Engine::Input::IsKeyPressed(Engine::Key::K)) {
            m_SquarePosition.y -= m_SquareMoveSpeed * ts;
        }

        m_Camera.SetPosition(m_CameraPosition);
        m_Camera.SetRotation(m_CameraRotation);

        Engine::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        Engine::RenderCommand::Clear();
        Engine::Renderer::BeginScene(m_Camera);
//        Engine::Renderer::Submit(m_Shader, m_VertexArray);

//        Engine::MaterialRef material = new Engine::Material(m_FlatShader);

        static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

        std::dynamic_pointer_cast<Engine::OpenGLShader>(m_FlatShader)->Bind();
        std::dynamic_pointer_cast<Engine::OpenGLShader>(m_FlatShader)->UploadUniformFloat3("u_Color", m_SquareColor);

        glm::vec4 redColor(0.8f, 0.2f, 0.3f, 1.0f);
        for (int j = 0; j < 20; j++) {
            for (int i = 0; i < 20; i++) {
                glm::vec3 pos(i * 0.11f, j * 0.11f, 0.0f);
                glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_SquarePosition + pos);
                Engine::Renderer::Submit(m_FlatShader, m_SquareVA, transform * scale);
            }
        }

        m_Texture->Bind();
        std::dynamic_pointer_cast<Engine::OpenGLShader>(m_TextureShader)->Bind();
        std::dynamic_pointer_cast<Engine::OpenGLShader>(m_TextureShader)->UploadUniformFloat3("u_Color", m_SquareColor);
        Engine::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

        Engine::Renderer::EndScene();
    }
    virtual void OnImGuiRender() override {
        ImGui::Begin("Settings");
        ImGui::ColorEdit3("Square color", glm::value_ptr(m_SquareColor));
        ImGui::End();
    }

private:
    Engine::Ref<Engine::Shader> m_Shader;
    Engine::Ref<Engine::Shader> m_FlatShader;
    Engine::Ref<Engine::Shader> m_TextureShader;

    Engine::Ref<Engine::Texture2D> m_Texture;

    Engine::Ref<Engine::VertexArray> m_VertexArray;
    Engine::Ref<Engine::VertexArray> m_SquareVA;

    Engine::OrthographicCamera m_Camera;
    glm::vec3 m_CameraPosition{0.0f};
    float m_CameraRotation = 0.0f;

    float m_CameraMoveSpeed = 2.0f;
    float m_CameraRotationSpeed = 45.0f;

    glm::vec3 m_SquarePosition{0.0f};
    float m_SquareMoveSpeed = 1.0f;
    glm::vec3 m_SquareColor{0.2f, 0.3f, 0.8f};
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