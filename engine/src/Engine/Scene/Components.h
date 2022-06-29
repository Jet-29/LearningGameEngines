#pragma once

#include "Engine/Renderer/Camera.h"

namespace Engine {

    struct TagComponent {
        std::string Tag;
    };

    struct TransformComponent {
        glm::mat4 Transform{1.0f};
    };

    struct SpriteRendererComponent {
        glm::vec4 Color{1.0f};
    };

    struct CameraComponent {
        Engine::Camera Camera;
        bool Primary = true;
    };

} // Engine