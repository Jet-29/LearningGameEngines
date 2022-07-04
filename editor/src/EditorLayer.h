#pragma once

#include "Engine.h"
#include "Panels/SceneHierarchyPanel.h"

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

        void NewScene();
        void OpenScene();
        void SaveSceneAs();

        Ref<FrameBuffer> m_FrameBuffer;

        Ref<Scene> m_ActiveScene;

        glm::vec2 m_ViewportSize;
        bool m_ViewportFocussed, m_ViewportHovered;
        glm::vec2 m_ViewportBounds[2];

        EditorCamera m_EditorCamera;
        Entity m_HoveredEntity;

        SceneHierarchyPanel m_SceneHierarchyPanel;

        int m_GizmoType = -1;
    };
}