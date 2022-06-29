#pragma once

#include "Engine.h"
namespace Engine {
    class EditorLayer : public Layer {
    public:
        EditorLayer() : Layer("Example 2D"), m_CameraController(1280.0f / 720.0f, true) {}
        virtual ~EditorLayer() override = default;

        virtual void OnAttach() override;

        virtual void OnUpdate(TimeStep dt) override;
        virtual void OnImGuiRender() override;
        virtual void OnEvent(Event& event) override;

    private:
        OrthographicCameraController m_CameraController;
        Ref<FrameBuffer> m_FrameBuffer;

        Ref<Scene> m_ActiveScene;
        Entity m_SquareEntity;
        Entity m_CameraEntity;
        Entity m_SecondCameraEntity;

        bool m_PrimaryCamera = true;

        glm::vec2 m_ViewportSize;
        bool m_ViewportFocussed, m_ViewportHovered;

        Ref<Texture2D> m_CheckerboardTexture;
    };
}