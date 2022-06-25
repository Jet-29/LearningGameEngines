#pragma once

#include <Engine.h>

class ExampleLayer : public Engine::Layer {
public:
    ExampleLayer() : Layer("Example"), m_CameraController(1280.0f / 720.0f, true) {}
    virtual void OnAttach() override;
    virtual void OnUpdate(Engine::TimeStep ts) override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent(Engine::Event &event) override;

private:
    Engine::ShaderLibrary m_ShaderLibrary;
    Engine::Ref<Engine::Shader> m_Shader;
    Engine::Ref<Engine::Shader> m_FlatShader;

    Engine::Ref<Engine::Texture2D> m_Texture, m_ChernoLogo;

    Engine::Ref<Engine::VertexArray> m_VertexArray;
    Engine::Ref<Engine::VertexArray> m_SquareVA;

    Engine::OrthographicCameraController m_CameraController;

    glm::vec3 m_SquarePosition{0.0f};
    float m_SquareMoveSpeed = 1.0f;
    glm::vec3 m_SquareColor{0.2f, 0.3f, 0.8f};

};
