#include "EnginePch.h"
#include "OrthographicCameraController.h"
#include "Engine/Core/KeyCodes.h"
#include "Engine/Core/Input.h"

namespace Engine {
    OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation) : m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation) {

    }
    void OrthographicCameraController::OnUpdate(TimeStep ts) {
        if (Input::IsKeyPressed(Key::A)) {
            m_CameraPosition.x -= m_CameraTranslationSpeed * m_ZoomLevel * ts;
        }
        if (Input::IsKeyPressed(Key::D)) {
            m_CameraPosition.x += m_CameraTranslationSpeed * m_ZoomLevel * ts;
        }
        if (Input::IsKeyPressed(Key::W)) {
            m_CameraPosition.y += m_CameraTranslationSpeed * m_ZoomLevel * ts;
        }
        if (Input::IsKeyPressed(Key::S)) {
            m_CameraPosition.y -= m_CameraTranslationSpeed * m_ZoomLevel * ts;
        }
        m_Camera.SetPosition(m_CameraPosition);

        if (m_Rotation) {
            if (Input::IsKeyPressed(Key::Q)) {
                m_CameraRotation += m_CameraRotationSpeed * ts;
            }
            if (Input::IsKeyPressed(Key::E)) {
                m_CameraRotation -= m_CameraRotationSpeed * ts;
            }
            m_Camera.SetRotation(m_CameraRotation);
        }
    }
    void OrthographicCameraController::OnEvent(Event &e) {
        EventDispatcher eventDispatcher(e);
        eventDispatcher.Dispatch<MouseScrolledEvent>(ENGINE_BIND_EVENT_FN(OnMouseScrolled));
        eventDispatcher.Dispatch<WindowResizeEvent>(ENGINE_BIND_EVENT_FN(OnWindowResize));
    }
    bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent &e) {
        m_ZoomLevel -= e.GetYOffset() * m_CameraZoomSpeed;
        m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
        return false;
    }
    bool OrthographicCameraController::OnWindowResize(WindowResizeEvent &e) {
        m_AspectRatio = (float) e.GetWidth() / (float) e.GetHeight();
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
        return false;
    }
} // Engine