#pragma once

#include <Engine.h>

class Example2D : public Engine::Layer {
public:
    Example2D() : Engine::Layer("Example 2D"), m_CameraController(1280.0f / 720.0f, true) {}
    virtual ~Example2D() override = default;

    virtual void OnAttach() override;

    virtual void OnUpdate(Engine::TimeStep dt) override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent(Engine::Event &event) override;

private:
    Engine::OrthographicCameraController m_CameraController;

    Engine::Ref<Engine::VertexArray> m_SquareVA;
    Engine::Ref<Engine::Shader> m_FlatColorShader;

    Engine::Ref<Engine::Texture2D> m_CheckerboardTexture;

    glm::vec4 m_SquareColor = {0.2f, 0.3f, 0.8f, 1.0f};
};
