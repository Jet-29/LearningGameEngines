#include "Example2D.h"
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

void Example2D::OnAttach() {
    m_CheckerboardTexture = Engine::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Example2D::OnUpdate(Engine::TimeStep dt) {
    m_CameraController.OnUpdate(dt);

    Engine::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
    Engine::RenderCommand::Clear();
    Engine::Renderer2D::BeginScene(m_CameraController.GetCamera());

    Engine::Renderer2D::DrawQuad({-1.0f, 0.0f}, {0.8f, 0.8f}, {0.8f, 0.2f, 0.3f, 1.0f});
    Engine::Renderer2D::DrawQuad({0.5f, -0.5f}, {0.5f, 0.75f}, {0.2f, 0.3f, 0.8f, 1.0f});

    Engine::Renderer2D::DrawQuad({0.0f, 0.0f, -0.1f}, {10.0f, 10.0f}, {0.7f, 1.0f, 0.8f, 1.0f}, m_CheckerboardTexture);

    Engine::Renderer2D::EndScene();

}
void Example2D::OnImGuiRender() {
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("2D Square color", glm::value_ptr(m_SquareColor));
    ImGui::End();
}
void Example2D::OnEvent(Engine::Event &event) {
    m_CameraController.OnEvent(event);
}

