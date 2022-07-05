#pragma once

#include "Engine.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"

namespace Engine {
    class EditorLayer : public Layer {
    public:
        EditorLayer() : Layer("Example 2D") {}
        virtual ~EditorLayer() override = default;

        virtual void OnAttach() override;

        virtual void OnUpdate(TimeStep dt) override;
        virtual void OnImGuiRender() override;
        virtual void OnEvent(Event& event) override;

    private:
        bool OnKeyPressed(KeyPressedEvent& event);
        bool OnMouseButtonPressed(MouseButtonPressedEvent& event);
        void OnOverlayRender();

        void NewScene();
        void OpenScene();
        void OpenScene(const std::filesystem::path& path);
        void SaveScene();
        void SaveSceneAs();

        void SerializeScene(Ref<Scene> scene, const std::filesystem::path& path);

        void OnScenePlay();
        void OnSceneStop();

        void OnDuplicateEntity();

        void UI_Toolbar();

        Ref<FrameBuffer> m_FrameBuffer;

        Ref<Scene> m_ActiveScene;
        Ref<Scene> m_EditorScene;
        std::filesystem::path m_EditorScenePath;

        glm::vec2 m_ViewportSize;
        bool m_ViewportFocussed, m_ViewportHovered;
        glm::vec2 m_ViewportBounds[2];

        EditorCamera m_EditorCamera;
        Entity m_HoveredEntity;

        Ref<Texture2D> m_PlayIcon;
        Ref<Texture2D> m_StopIcon;

        SceneHierarchyPanel m_SceneHierarchyPanel;
        ContentBrowserPanel m_ContentBrowserPanel;

        int m_GizmoType = -1;

        bool m_ShowPhysicsColliders = false;

        enum class SceneState {
            Edit,
            Play
        };
        SceneState m_SceneState = SceneState::Edit;
    };
}