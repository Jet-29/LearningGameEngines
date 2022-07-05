#include "SceneCamera.h"

namespace Engine {
    SceneCamera::SceneCamera() {
        CalculateProjection();
    }
    void SceneCamera::SetPerspective(float fov, float nearClip, float farClip) {
        m_ProjectionType = ProjectionType::Perspective;
        m_PerspectiveFOV = fov;
        m_PerspectiveNear = nearClip;
        m_PerspectiveFar = farClip;
        CalculateProjection();
    }
    void SceneCamera::SetOrthographic(float size, float nearClip, float farClip) {
        m_ProjectionType = ProjectionType::Orthographic;
        m_OrthographicSize = size;
        m_OrthographicNear = nearClip;
        m_OrthographicFar = farClip;
        CalculateProjection();
    }
    void SceneCamera::SetViewportSize(uint32_t width, uint32_t height) {
        ENGINE_CORE_ASSERT(width > 0 && height > 0);
        m_AspectRatio = (float) width / (float) height;
        CalculateProjection();
    }
    void SceneCamera::CalculateProjection() {
        if (m_ProjectionType == ProjectionType::Perspective) {
            m_Projection = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
        } else if (m_ProjectionType == ProjectionType::Orthographic) {
            float orthographicHorizontal = m_OrthographicSize * m_AspectRatio * 0.5f;
            float orthographicVertical = m_OrthographicSize * 0.5f;

            m_Projection = glm::ortho(-orthographicHorizontal, orthographicHorizontal, -orthographicVertical, orthographicVertical, m_OrthographicNear, m_OrthographicFar);
        }
    }
    void SceneCamera::SetPerspectiveFOV(float fov) {
        m_PerspectiveFOV = fov;
        CalculateProjection();
    }
    void SceneCamera::SetPerspectiveNearClip(float nearClip) {
        m_PerspectiveNear = nearClip;
        CalculateProjection();
    }
    void SceneCamera::SetPerspectiveFarClip(float farClip) {
        m_PerspectiveFar = farClip;
        CalculateProjection();
    }
    void SceneCamera::SetOrthographicSize(float size) {
        m_OrthographicSize = size;
        CalculateProjection();
    }
    void SceneCamera::SetOrthographicNearClip(float nearClip) {
        m_OrthographicNear = nearClip;
        CalculateProjection();
    }
    void SceneCamera::SetOrthographicFarClip(float farClip) {
        m_OrthographicFar = farClip;
        CalculateProjection();
    }
    void SceneCamera::SetProjectionType(SceneCamera::ProjectionType type) {
        m_ProjectionType = type;
        CalculateProjection();
    }

} // Engine