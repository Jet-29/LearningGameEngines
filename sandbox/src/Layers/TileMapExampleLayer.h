#pragma once

#include <Engine.h>

class TileMapExampleLayer : public Engine::Layer {
public:
    TileMapExampleLayer() : Engine::Layer("Example 2D"), m_CameraController(1280.0f / 720.0f, true) {}
    virtual void OnAttach() override;
    virtual void OnUpdate(Engine::TimeStep dt) override;
    virtual void OnEvent(Engine::Event& event) override;
private:
    Engine::OrthographicCameraController m_CameraController;

    Engine::Ref<Engine::Texture2D> m_SpriteSheet;

    uint32_t m_MapWidth, m_MapHeight;
    std::unordered_map<char, Engine::Ref<Engine::SubTexture2D>> s_TextureMap;
};
