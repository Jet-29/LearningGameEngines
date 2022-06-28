#include "EditorLayer.h"
#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"

namespace Engine {
    void EditorLayer::OnAttach() {
        m_CheckerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");

        FrameBufferSpecification fbSpec;
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
        m_FrameBuffer = FrameBuffer::Create(fbSpec);
    }

    void EditorLayer::OnUpdate(TimeStep dt) {
        m_CameraController.OnUpdate(dt);
        Renderer2D::ResetStats();

        m_FrameBuffer->Bind();
        RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        RenderCommand::Clear();

        static float rotation = 0.0f;
        rotation += dt * 0.5f;

        Renderer2D::BeginScene(m_CameraController.GetCamera());

        Renderer2D::DrawRotatedQuad({1.0f, 0.0f, -1.0f}, {0.8f, 0.8f}, -0.785f, {0.8f, 0.2f, 0.3f, 1.0f});

        Renderer2D::DrawQuad({-1.0f, 0.0f, -1.0f}, {0.8f, 0.8f}, {0.8f, 0.2f, 0.3f, 1.0f});
        Renderer2D::DrawQuad({0.5f, -0.5f, -1.0f}, {0.5f, 0.75f}, {0.2f, 0.3f, 0.8f, 1.0f});

        Renderer2D::DrawQuad({0.0f, 0.0f, -1.999f}, {20.0f, 20.0f}, m_CheckerboardTexture, 10.0f);
        Renderer2D::DrawRotatedQuad({-2.0f, 0.0f, -1.0f}, {0.5f, 0.5f}, rotation, {0.7f, 1.0f, 0.8f, 1.0f}, m_CheckerboardTexture, 20.0f);

        for (float y = -5.0f; y < 5.0f; y += 0.5f) {
            for (float x = -5.0f; x < 5.0f; x += 0.5f) {
                glm::vec4 color{(x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.75f};
                Renderer2D::DrawQuad({x, y, -0.9f}, {0.45f, 0.45f}, color);
            }
        }
        Renderer2D::EndScene();

        m_FrameBuffer->Unbind();

    }
    void EditorLayer::OnImGuiRender() {
        static bool showDockSpace = true;
        static ImGuiDockNodeFlags dockSpace_flags = ImGuiDockNodeFlags_None;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        if (dockSpace_flags & ImGuiDockNodeFlags_PassthruCentralNode) {
            window_flags |= ImGuiWindowFlags_NoBackground;
        }

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &showDockSpace, window_flags);
        ImGui::PopStyleVar(3);

        ImGuiID dockSpace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockSpace_id, ImVec2(0.0f, 0.0f), dockSpace_flags);

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Exit")) {
                    Application::Get().Close();
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        ImGui::Begin("Settings");
        auto stats = Renderer2D::GetStats();
        ImGui::Text("Renderer2D Stats:");
        ImGui::Text("Draw calls: %d", stats.DrawCalls);
        ImGui::Text("Quad count: %d", stats.QuadCount);
        ImGui::Text("Vertex count: %d", stats.GetTotalVertexCount());
        ImGui::Text("Index count: %d", stats.GetTotalIndexCount());
        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
        ImGui::Begin("Viewport");
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

        if (m_ViewportSize != *((glm::vec2*) &viewportPanelSize)) {
            m_FrameBuffer->Resize((uint32_t) viewportPanelSize.x, (uint32_t) viewportPanelSize.y);
            m_ViewportSize = {viewportPanelSize.x, viewportPanelSize.y};
            m_CameraController.ResizeBounds(viewportPanelSize.x, viewportPanelSize.y);
        }
        uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
        ImGui::Image((void*) (uint64_t) textureID, {viewportPanelSize.x, viewportPanelSize.y}, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();
        ImGui::PopStyleVar();

        ImGui::End();
    }
    void EditorLayer::OnEvent(Event& event) {
        m_CameraController.OnEvent(event);
    }

}