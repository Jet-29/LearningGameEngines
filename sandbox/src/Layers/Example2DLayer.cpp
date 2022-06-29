#include "Example2DLayer.h"
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

void Example2DLayer::OnAttach() {
    m_CheckerboardTexture = Engine::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Example2DLayer::OnUpdate(Engine::TimeStep dt) {
    m_CameraController.OnUpdate(dt);
    Engine::Renderer2D::ResetStats();

    Engine::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
    Engine::RenderCommand::Clear();

    static float rotation = 0.0f;
    rotation += dt * 0.5f;

    Engine::Renderer2D::BeginScene(m_CameraController.GetCamera());

    Engine::Renderer2D::DrawRotatedQuad({1.0f, 0.0f, -1.0f}, {0.8f, 0.8f}, -0.785f, {0.8f, 0.2f, 0.3f, 1.0f});

    Engine::Renderer2D::DrawQuad({-1.0f, 0.0f, -1.0f}, {0.8f, 0.8f}, {0.8f, 0.2f, 0.3f, 1.0f});
    Engine::Renderer2D::DrawQuad({0.5f, -0.5f, -1.0f}, {0.5f, 0.75f}, {0.2f, 0.3f, 0.8f, 1.0f});

    Engine::Renderer2D::DrawQuad({0.0f, 0.0f, -1.999f}, {20.0f, 20.0f}, m_CheckerboardTexture, 10.0f);
    Engine::Renderer2D::DrawRotatedQuad({-2.0f, 0.0f, -1.0f}, {0.5f, 0.5f}, rotation, {0.7f, 1.0f, 0.8f, 1.0f}, m_CheckerboardTexture, 20.0f);

    for (float y = -5.0f; y < 5.0f; y += 0.5f) {
        for (float x = -5.0f; x < 5.0f; x += 0.5f) {
            glm::vec4 color{(x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.75f};
            Engine::Renderer2D::DrawQuad({x, y, -0.9f}, {0.45f, 0.45f}, color);
        }
    }
    Engine::Renderer2D::EndScene();

}
void Example2DLayer::OnImGuiRender() {

    ImGui::Begin("Settings");

    auto stats = Engine::Renderer2D::GetStats();
    ImGui::Text("Renderer2D Stats:");
    ImGui::Text("Draw calls: %d", stats.DrawCalls);
    ImGui::Text("Quad count: %d", stats.QuadCount);
    ImGui::Text("Vertex count: %d", stats.GetTotalVertexCount());
    ImGui::Text("Index count: %d", stats.GetTotalIndexCount());

    ImGui::End();

}
void Example2DLayer::OnEvent(Engine::Event& event) {
    m_CameraController.OnEvent(event);
}

