#pragma once

#include <Engine.h>
#include "Particle System/ParticleSystem.h"

class ParticleSystemLayer : public Engine::Layer {
public:
    ParticleSystemLayer() : Engine::Layer("Example 2D"), m_CameraController(1280.0f / 720.0f, true) {}
    virtual void OnAttach() override;
    virtual void OnUpdate(Engine::TimeStep dt) override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent(Engine::Event& event) override;
private:
    Engine::OrthographicCameraController m_CameraController;

    ParticleProps m_Particle;
    ParticleSystem m_ParticleSystem;
};
