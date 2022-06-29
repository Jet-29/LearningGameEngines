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

        m_ActiveScene = CreateRef<Scene>();

        auto square = m_ActiveScene->CreateEntity("Square");
        square.AddComponent<SpriteRendererComponent>(glm::vec4{0.0f, 1.0f, 0.0f, 1.0f});
        m_SquareEntity = square;

        m_CameraEntity = m_ActiveScene->CreateEntity("Camera Entity");
        m_CameraEntity.AddComponent<CameraComponent>(glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f));

        m_SecondCameraEntity = m_ActiveScene->CreateEntity("Clip-Space Camera Entity");
        auto& cc = m_SecondCameraEntity.AddComponent<CameraComponent>(glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f));
        cc.Primary = false;
    }

    void EditorLayer::OnUpdate(TimeStep dt) {

        if (FrameBufferSpecification spec = m_FrameBuffer->GetSpecification();
            m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y)) {
            m_FrameBuffer->Resize(m_ViewportSize.x, m_ViewportSize.y);
            m_CameraController.ResizeBounds(m_ViewportSize.x, m_ViewportSize.y);
        }

        if (m_ViewportFocussed) {
            m_CameraController.OnUpdate(dt);
        }
        Renderer2D::ResetStats();

        m_FrameBuffer->Bind();
        RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        RenderCommand::Clear();

        m_ActiveScene->OnUpdate(dt);

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

        if (m_SquareEntity) {
            ImGui::Separator();
            ImGui::Text("%s", m_SquareEntity.GetComponent<TagComponent>().Tag.c_str());
            auto& squareColor = m_SquareEntity.GetComponent<SpriteRendererComponent>().Color;
            ImGui::ColorEdit4("Square color", glm::value_ptr(squareColor));
        }

        ImGui::DragFloat3("Camera A Transform", glm::value_ptr(m_CameraEntity.GetComponent<TransformComponent>().Transform[3]));
        ImGui::DragFloat3("Camera B Transform", glm::value_ptr(m_SecondCameraEntity.GetComponent<TransformComponent>().Transform[3]));

        if (ImGui::Checkbox("Camera toggle", &m_PrimaryCamera)) {
            m_CameraEntity.GetComponent<CameraComponent>().Primary = m_PrimaryCamera;
            m_SecondCameraEntity.GetComponent<CameraComponent>().Primary = !m_PrimaryCamera;
        }

        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
        ImGui::Begin("Viewport");
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

        m_ViewportFocussed = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();
        Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocussed || !m_ViewportHovered);

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