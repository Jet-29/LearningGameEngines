#include "EditorLayer.h"
#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"

#include "Engine/Scene/SceneSerializer.h"
#include "Engine/Utils/PlatformUtils.h"

#include <ImGuizmo.h>

#include "Engine/Math/Math.h"

namespace Engine {

    extern const std::filesystem::path g_AssetPath;

    void EditorLayer::OnAttach() {

        m_PlayIcon = Texture2D::Create("assets/Resources/Icons/PlayButton.png");
        m_SimulateIcon = Texture2D::Create("assets/Resources/Icons/SimulateButton.png");
        m_StopIcon = Texture2D::Create("assets/Resources/Icons/StopButton.png");

        FrameBufferSpecification fbSpec;
        fbSpec.Attachments = {FrameBufferTextureFormat::RGBA8, FrameBufferTextureFormat::RED_INTEGER, FrameBufferTextureFormat::Depth};
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
        m_FrameBuffer = FrameBuffer::Create(fbSpec);

        m_ActiveScene = CreateRef<Scene>();

        m_SceneHierarchyPanel.SetContext(m_ActiveScene);

        auto commandLineArgs = Application::Get().GetSpecification().CommandLineArgs;
        if (commandLineArgs.Count > 1) {
            auto sceneFilePath = commandLineArgs[1];
            SceneSerializer serializer(m_ActiveScene);
            serializer.Deserialize(sceneFilePath);
        }

        m_EditorCamera = EditorCamera(30.0f, 16.0f / 9.0f, 0.1f, 10000.0f);
    }

    void EditorLayer::OnUpdate(TimeStep dt) {

        if (FrameBufferSpecification spec = m_FrameBuffer->GetSpecification();
            m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y)) {
            m_FrameBuffer->Resize(m_ViewportSize.x, m_ViewportSize.y);

            m_ActiveScene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
            m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
        }

        Renderer2D::ResetStats();

        m_FrameBuffer->Bind();
        RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        RenderCommand::Clear();
        m_FrameBuffer->ClearAttachment(1, -1);

        switch (m_SceneState) {
            case SceneState::Edit: {
                m_EditorCamera.OnUpdate(dt);
                m_ActiveScene->OnUpdateEditor(dt, m_EditorCamera);
                break;
            }
            case SceneState::Simulate: {
                m_EditorCamera.OnUpdate(dt);
                m_ActiveScene->OnUpdateSimulation(dt, m_EditorCamera);
                break;
            }
            case SceneState::Play: {
                m_ActiveScene->OnUpdateRuntime(dt);
                break;
            }
        }

        auto [mx, my] = ImGui::GetMousePos();
        mx -= m_ViewportBounds[0].x;
        my -= m_ViewportBounds[0].y;
        glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
        my = viewportSize.y - my;
        int mouseX = (int) mx;
        int mouseY = (int) my;

        if (mouseX >= 0 && mouseY >= 0 && mouseX < (int) viewportSize.x && mouseY < (int) viewportSize.y) {
            int pixelData = m_FrameBuffer->ReadPixel(1, mouseX, mouseY);
            m_HoveredEntity = pixelData == -1 ? Entity{} : Entity{(entt::entity) pixelData, m_ActiveScene.get()};
        }

        OnOverlayRender();
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
        m_ContentBrowserPanel.OnImGuiRender();

        ImGui::Begin("Settings");
        ImGui::Checkbox("Show Physics Colliders", &m_ShowPhysicsColliders);
        ImGui::End();

        ImGui::Begin("Renderer stats");
        std::string name = "None";
        if (m_HoveredEntity) {
            name = m_HoveredEntity.GetComponent<TagComponent>().Tag;
        }

        ImGui::Text("Hovered Entity: %s", name.c_str());

        auto stats = Renderer2D::GetStats();
        ImGui::Text("Renderer2D Stats:");
        ImGui::Text("Draw calls: %d", stats.DrawCalls);
        ImGui::Text("Quad count: %d", stats.QuadCount);
        ImGui::Text("Vertex count: %d", stats.GetTotalVertexCount());
        ImGui::Text("Index count: %d", stats.GetTotalIndexCount());
        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
        ImGui::Begin("Viewport");
        auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
        auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
        auto viewportOffset = ImGui::GetWindowPos();
        m_ViewportBounds[0] = {viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y};
        m_ViewportBounds[1] = {viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y};

        m_ViewportFocussed = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();
        Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocussed && !m_ViewportHovered);

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        m_ViewportSize = {viewportPanelSize.x, viewportPanelSize.y};

        uint64_t textureID = m_FrameBuffer->GetColorAttachmentRendererID(0);
        ImGui::Image((void*) textureID, {viewportPanelSize.x, viewportPanelSize.y}, ImVec2(0, 1), ImVec2(1, 0));

        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
                const auto* path = (const wchar_t*) payload->Data;
                OpenScene(std::filesystem::path(g_AssetPath) / path);
            }
            ImGui::EndDragDropTarget();
        }

        Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
        if (selectedEntity && m_GizmoType != -1) {
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();
            auto windowWidth = (float) ImGui::GetWindowWidth();
            auto windowHeight = (float) ImGui::GetWindowHeight();
            ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

            const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
            glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

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

        UI_Toolbar();

        ImGui::End();
    }
    void EditorLayer::OnEvent(Event& event) {
        m_EditorCamera.OnEvent(event);

        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<KeyPressedEvent>(ENGINE_BIND_EVENT_FN(OnKeyPressed));
        dispatcher.Dispatch<MouseButtonPressedEvent>(ENGINE_BIND_EVENT_FN(OnMouseButtonPressed));
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
                if (controlPressed) {
                    if (shiftPressed) {
                        SaveSceneAs();
                    } else {
                        SaveScene();
                    }
                }
                break;
            }
            case Key::D: {
                if (controlPressed) {
                    OnDuplicateEntity();
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
        m_ActiveScene = CreateRef<Scene>();;
        m_ActiveScene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
        m_EditorScenePath = std::filesystem::path{};
    }
    void EditorLayer::OpenScene() {
        OpenScene(FileDialogs::OpenFile("Engine Scene (*.engine)\0*.engine\0"));
    }
    void EditorLayer::OpenScene(const std::filesystem::path& path) {
        if (m_SceneState != SceneState::Edit) {
            OnSceneStop();
        }

        Ref<Scene> newScene = CreateRef<Scene>();
        SceneSerializer serializer(newScene);
        if (serializer.Deserialize(path.string())) {
            m_EditorScene = newScene;
            m_EditorScene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
            m_SceneHierarchyPanel.SetContext(m_EditorScene);

            m_ActiveScene = m_EditorScene;
            m_EditorScenePath = path;
        }
    }
    void EditorLayer::SaveScene() {
        if (!m_EditorScenePath.empty()) {
            SerializeScene(m_EditorScene, m_EditorScenePath);
        } else {
            SaveSceneAs();
        }
    }
    void EditorLayer::SaveSceneAs() {
        std::string filepath = FileDialogs::SaveFile("Engine Scene (*.engine)\0*.engine\0");
        if (!filepath.empty()) {
            SerializeScene(m_EditorScene, filepath);
            m_EditorScenePath = filepath;
        }
    }
    void EditorLayer::SerializeScene(Ref<Scene> scene, const std::filesystem::path& path) {
        SceneSerializer serializer(scene);
        serializer.Serialize(path.string());
    }
    bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& event) {
        if (event.GetMouseButton() == Mouse::ButtonLeft && m_ViewportHovered && !Input::IsKeyPressed(Key::LeftAlt) && (!ImGuizmo::IsOver() || !m_SceneHierarchyPanel.GetSelectedEntity() || m_GizmoType == -1)) {
            m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
        }
        return false;
    }
    void EditorLayer::UI_Toolbar() {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        auto& colors = ImGui::GetStyle().Colors;
        const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
        const auto& buttonActive = colors[ImGuiCol_ButtonActive];
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

        ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

        bool toolbarEnable = (bool) m_ActiveScene;

        float size = ImGui::GetWindowHeight() - 4.0f;
        {
            ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
            Ref<Texture2D> icon = (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate) ? m_PlayIcon : m_StopIcon;
            if (ImGui::ImageButton((ImTextureID) (uint64_t) icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0) && toolbarEnable) {
                if (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate)
                    OnScenePlay();
                else if (m_SceneState == SceneState::Play)
                    OnSceneStop();
            }
        }
        {
            ImGui::SameLine();
//            ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
            Ref<Texture2D> icon = (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play) ? m_SimulateIcon : m_StopIcon;
            if (ImGui::ImageButton((ImTextureID) (uint64_t) icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0) && toolbarEnable) {
                if (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play)
                    OnSceneSimulate();
                else if (m_SceneState == SceneState::Simulate)
                    OnSceneStop();
            }
        }

        ImGui::PopStyleColor(3);
        ImGui::PopStyleVar(2);

        ImGui::End();
    }
    void EditorLayer::OnScenePlay() {
        if (m_SceneState == SceneState::Simulate) {
            OnSceneStop();
        }

        m_SceneState = SceneState::Play;

        m_ActiveScene = Scene::Copy(m_EditorScene);
        m_ActiveScene->OnRuntimeStart();
        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
    }
    void EditorLayer::OnSceneStop() {
        if (m_SceneState == SceneState::Play) {
            m_ActiveScene->OnRuntimeStop();
        } else if (m_SceneState == SceneState::Simulate) {
            m_ActiveScene->OnSimulationStop();
        } else {
            ENGINE_CORE_ASSERT(false, "What?");
        }

        m_SceneState = SceneState::Edit;

        m_ActiveScene = m_EditorScene;
        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
    }
    void EditorLayer::OnDuplicateEntity() {
        if (m_SceneState != SceneState::Edit) return;

        Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
        if (selectedEntity) {
            m_EditorScene->DuplicateEntity(selectedEntity);
        }
    }
    void EditorLayer::OnOverlayRender() {

        if (m_SceneState == SceneState::Play) {
            Entity camera = m_ActiveScene->GetPrimaryCameraEntity();
            if (!camera) {
                return;
            }
            Renderer2D::BeginScene(camera.GetComponent<CameraComponent>().Camera, camera.GetComponent<TransformComponent>().GetTransform());
        } else {
            Renderer2D::BeginScene(m_EditorCamera);
        }
        if (m_ShowPhysicsColliders) {
            {
                auto view = m_ActiveScene->GetAllEntitiesWith<TransformComponent, CircleCollider2DComponent>();
                for (auto entity : view) {
                    auto [tc, cc2d] = view.get<TransformComponent, CircleCollider2DComponent>(entity);
                    glm::vec3 translation = tc.Translation + glm::vec3(cc2d.Offset, 0.001f);
                    glm::vec3 scale = tc.Scale * glm::vec3(cc2d.Radius * 2.0f);

                    glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation) * glm::scale(glm::mat4(1.0f), scale);

                    Renderer2D::DrawCircle(transform, glm::vec4(0, 1, 0, 1), 0.005f, 0.001f);
                }
            }
            {
                auto view = m_ActiveScene->GetAllEntitiesWith<TransformComponent, BoxCollider2DComponent>();
                for (auto entity : view) {
                    auto [tc, bc2d] = view.get<TransformComponent, BoxCollider2DComponent>(entity);
                    glm::vec3 translation = tc.Translation + glm::vec3(bc2d.Offset, 0.001f);
                    float rotation = tc.Rotation.z;
                    glm::vec3 scale = tc.Scale * glm::vec3(bc2d.Size * 2.0f, 1.0f);

                    glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation) * glm::rotate(glm::mat4(1.0f), rotation, {0, 0, 1}) * glm::scale(glm::mat4(1.0f), scale);

                    Renderer2D::DrawRect(transform, glm::vec4(0, 1, 0, 1));
                }
            }
        }

        Renderer2D::EndScene();
    }
    void EditorLayer::OnSceneSimulate() {
        if (m_SceneState == SceneState::Play) {
            OnSceneStop();
        }

        m_SceneState = SceneState::Simulate;

        m_ActiveScene = Scene::Copy(m_EditorScene);
        m_ActiveScene->OnSimulationStart();
        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
    }

}
