#include "ParticleSystemLayer.h"

void ParticleSystemLayer::OnAttach() {
    m_Particle.ColorBegin = {254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f};
    m_Particle.ColorEnd = {254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f};
    m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
    m_Particle.LifeTime = 1.0f;
    m_Particle.Velocity = {0.0f, 0.0f};
    m_Particle.VelocityVariation = {3.0f, 3.0f};
    m_Particle.Position = {0.0f, 0.0f};
}
void ParticleSystemLayer::OnUpdate(Engine::TimeStep dt) {
    m_CameraController.OnUpdate(dt);

    Engine::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
    Engine::RenderCommand::Clear();

    if (Engine::Input::IsMouseButtonPressed(Engine::Mouse::ButtonLeft)) {
        auto [x, y] = Engine::Input::GetMousePosition();
        auto width = Engine::Application::Get().GetWindow().GetWidth();
        auto height = Engine::Application::Get().GetWindow().GetHeight();

        auto bounds = m_CameraController.GetBounds();
        auto pos = m_CameraController.GetCamera().GetPosition();
        x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
        y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
        m_Particle.Position = {x + pos.x, y + pos.y};
        for (int i = 0; i < 5; i++)
            m_ParticleSystem.Emit(m_Particle);
    }
    m_ParticleSystem.OnUpdate(dt);
    m_ParticleSystem.OnRender(m_CameraController.GetCamera());
}
void ParticleSystemLayer::OnImGuiRender() {
    Layer::OnImGuiRender();
}
void ParticleSystemLayer::OnEvent(Engine::Event& event) {
    m_CameraController.OnEvent(event);
}
