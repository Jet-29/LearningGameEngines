#pragma once

#include <glm/glm.hpp>

namespace Engine {

    class OrthographicCamera {
    public:
        OrthographicCamera(float left, float right, float bottom, float top);
        void SetProjection(float left, float right, float bottom, float top);

        const glm::vec3& GetPosition();
        void SetPosition(const glm::vec3& position);

        float GetRotation() const;
        void SetRotation(float rotation);

        const glm::mat4& GetProjectionMatrix() const;
        const glm::mat4& GetViewMatrix() const;
        const glm::mat4& GetViewProjectionMatrix() const;

    private:
        void RecalculateViewMatrix();

        glm::mat4 m_ProjectionMatrix;
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ViewProjectionMatrix;

        glm::vec3 m_Position{0.0f};
        float m_Rotation = 0.0f;

    };

} // Engine