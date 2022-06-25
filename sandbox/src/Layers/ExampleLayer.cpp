#include "ExampleLayer.h"

#include <Platforms/OpenGL/OpenGLShader.h>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

void ExampleLayer::OnAttach() {
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

    m_Shader = Engine::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);

    m_FlatShader = Engine::Shader::Create("assets/shaders/FlatColor.glsl");

    auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

    m_Texture = Engine::Texture2D::Create("assets/textures/Checkerboard.png");
    m_ChernoLogo = Engine::Texture2D::Create("assets/textures/ChernoLogo.png");
    std::dynamic_pointer_cast<Engine::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
}
void ExampleLayer::OnUpdate(Engine::TimeStep ts) {
    m_CameraController.OnUpdate(ts);

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

    Engine::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
    Engine::RenderCommand::Clear();
    Engine::Renderer::BeginScene(m_CameraController.GetCamera());
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

    auto textureShader = m_ShaderLibrary.Get("Texture");

    m_Texture->Bind(0);
    std::dynamic_pointer_cast<Engine::OpenGLShader>(textureShader)->Bind();
    std::dynamic_pointer_cast<Engine::OpenGLShader>(textureShader)->UploadUniformFloat3("u_Color", m_SquareColor);
    Engine::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

    m_ChernoLogo->Bind(0);
    Engine::Renderer::Submit(textureShader, m_SquareVA, glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 0.0f)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

    Engine::Renderer::EndScene();

}
void ExampleLayer::OnImGuiRender() {

    ImGui::Begin("Settings");
    ImGui::ColorEdit3("Square color", glm::value_ptr(m_SquareColor));
    ImGui::End();

}
void ExampleLayer::OnEvent(Engine::Event &event) {

    m_CameraController.OnEvent(event);

}
