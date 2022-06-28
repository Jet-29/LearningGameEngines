#pragma once

#include <Engine.h>

class EditorLayer : public Engine::Layer {
public:
    EditorLayer() : Engine::Layer("Example 2D"), m_CameraController(1280.0f / 720.0f, true) {}
    virtual ~EditorLayer() override = default;

    virtual void OnAttach() override;

    virtual void OnUpdate(Engine::TimeStep dt) override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent(Engine::Event& event) override;

private:
    Engine::OrthographicCameraController m_CameraController;

    Engine::Ref<Engine::Texture2D> m_CheckerboardTexture;

};
