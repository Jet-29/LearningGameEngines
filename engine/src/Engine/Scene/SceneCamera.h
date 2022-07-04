#pragma once

#include "Engine/Renderer/Camera.h"

namespace Engine {

    class SceneCamera : public Camera {
    public:

        enum class ProjectionType {
            Perspective, Orthographic
        };

        SceneCamera();
        virtual ~SceneCamera() override = default;

        void SetPerspective(float fov, float nearClip, float farClip);
        void SetOrthographic(float size, float nearClip, float farClip);
        void SetViewportSize(uint32_t width, uint32_t height);

        void SetPerspectiveFOV(float fov);
        float GetPerspectiveFOV() const { return m_PerspectiveFOV; }
        float GetPerspectiveNearClip() const { return m_PerspectiveNear; }
        void SetPerspectiveNearClip(float nearClip);
        float GetPerspectiveFarClip() const { return m_PerspectiveFar; }
        void SetPerspectiveFarClip(float farClip);

        void SetOrthographicSize(float size);
        float GetOrthographicSize() const { return m_OrthographicSize; }
        float GetOrthographicNearClip() const { return m_OrthographicNear; }
        void SetOrthographicNearClip(float nearClip);
        float GetOrthographicFarClip() const { return m_OrthographicFar; }
        void SetOrthographicFarClip(float farClip);

        ProjectionType GetProjectionType() const { return m_ProjectionType; }
        void SetProjectionType(ProjectionType type);

    private:
        void CalculateProjection();

        ProjectionType m_ProjectionType = ProjectionType::Orthographic;

        float m_PerspectiveFOV = glm::radians(45.0f);
        float m_PerspectiveNear = 0.001f;
        float m_PerspectiveFar = 10000.0f;

        float m_OrthographicSize = 10.0f;
        float m_OrthographicNear = -1.0f;
        float m_OrthographicFar = 1.0f;

        float m_AspectRatio = 0.0f;
    };

} // Engine