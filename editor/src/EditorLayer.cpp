#include "EditorLayer.h"
#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"

#include "Engine/Scene/SceneSerializer.h"
#include "Engine/Utils/PlatformUtils.h"

#include <ImGuizmo.h>

#include "Engine/Math/Math.h"

namespace Engine {

    void EditorLayer::OnAttach() {

        FrameBufferSpecification fbSpec;
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
        m_FrameBuffer = FrameBuffer::Create(fbSpec);

        m_ActiveScene = CreateRef<Scene>();

        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
    }

    void EditorLayer::OnUpdate(TimeStep dt) {

        if (FrameBufferSpecification spec = m_FrameBuffer->GetSpecification();
            m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y)) {
            m_FrameBuffer->Resize(m_ViewportSize.x, m_ViewportSize.y);

            m_ActiveScene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
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

        ImGuiIO& io = ImGui::GetIO();
        ImGuiStyle& style = ImGui::GetStyle();
        float minWinSizeX = style.WindowMinSize.x;
        style.WindowMinSize.x = 370.0f;

        ImGuiID dockSpace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockSpace_id, ImVec2(0.0f, 0.0f), dockSpace_flags);

        style.WindowMinSize.x = minWinSizeX;

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New", "Ctrl+N")) {
                    NewScene();
                }
                if (ImGui::MenuItem("Open...", "Ctrl+O")) {
                    OpenScene();
                }
                if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {
                    SaveSceneAs();
                }
                if (ImGui::MenuItem("Exit")) {
                    Application::Get().Close();
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        m_SceneHierarchyPanel.OnImGuiRender();

        ImGui::Begin("Renderer stats");
        auto stats = Renderer2D::GetStats();
        ImGui::Text("Renderer2D Stats:");
        ImGui::Text("Draw calls: %d", stats.DrawCalls);
        ImGui::Text("Quad count: %d", stats.QuadCount);
        ImGui::Text("Vertex count: %d", stats.GetTotalVertexCount());
        ImGui::Text("Index count: %d", stats.GetTotalIndexCount());
        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
        ImGui::Begin("Viewport");

        m_ViewportFocussed = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();
        Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocussed && !m_ViewportHovered);

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        m_ViewportSize = {viewportPanelSize.x, viewportPanelSize.y};

        uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
        ImGui::Image((void*) (uint64_t) textureID, {viewportPanelSize.x, viewportPanelSize.y}, ImVec2(0, 1), ImVec2(1, 0));

        Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
        if (selectedEntity && m_GizmoType != -1) {
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();
            auto windowWidth = (float) ImGui::GetWindowWidth();
            auto windowHeight = (float) ImGui::GetWindowHeight();
            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

            auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
            const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
            const glm::mat4& cameraProjection = camera.GetProjection();
            glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

            auto& tc = selectedEntity.GetComponent<TransformComponent>();
            glm::mat4 transform = tc.GetTransform();

            bool snap = Input::IsKeyPressed(Key::LeftControl);
            float snapValue = 0.5f;
            if (m_GizmoType == ImGuizmo::OPERATION::ROTATE) {
                snapValue = 45.0f;
            }
            float snapValues[3] = {snapValue, snapValue, snapValue};

            ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), (ImGuizmo::OPERATION) m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform), nullptr, snap ? snapValues : nullptr);

            if (ImGuizmo::IsUsing()) {
                glm::vec3 translation, rotation, scale;
                Math::DecomposeTransform(transform, translation, rotation, scale);

                glm::vec3 deltaRotation = rotation - tc.Rotation;

                tc.Translation = translation;
                tc.Rotation += deltaRotation;
                tc.Scale = scale;
            }
        }

        ImGui::End();
        ImGui::PopStyleVar();

        ImGui::End();
    }
    void EditorLayer::OnEvent(Event& event) {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<KeyPressedEvent>(ENGINE_BIND_EVENT_FN(OnKeyPressed));
    }
    bool EditorLayer::OnKeyPressed(KeyPressedEvent& event) {
        if (event.GetRepeatCount() != 0) return false;
        bool controlPressed = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
        bool shiftPressed = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
        switch (event.GetKeyCode()) {
            case Key::N: {
                if (controlPressed) {
                    NewScene();
                }
                break;
            }
            case Key::O: {
                if (controlPressed) {
                    OpenScene();
                }
                break;
            }
            case Key::S: {
                if (controlPressed && shiftPressed) {
                    SaveSceneAs();
                }
                break;
            }
            case Key::Q:m_GizmoType = -1;
                break;
            case Key::W:m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
                break;
            case Key::E:m_GizmoType = ImGuizmo::OPERATION::ROTATE;
                break;
            case Key::R:m_GizmoType = ImGuizmo::OPERATION::SCALE;
                break;
        }
        return false;
    }
    void EditorLayer::NewScene() {
        m_ActiveScene = CreateRef<Scene>();
        m_ActiveScene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
    }
    void EditorLayer::OpenScene() {
        std::string filepath = FileDialogs::OpenFile("Engine Scene (*.engine)\0*.engine\0");
        if (!filepath.empty()) {
            NewScene();
            SceneSerializer serializer(m_ActiveScene);
            serializer.Deserialize(filepath);
        }
    }
    void EditorLayer::SaveSceneAs() {
        std::string filepath = FileDialogs::SaveFile("Engine Scene (*.engine)\0*.engine\0");
        if (!filepath.empty()) {
            SceneSerializer serializer(m_ActiveScene);
            serializer.Serialize(filepath);
        }
    }
}