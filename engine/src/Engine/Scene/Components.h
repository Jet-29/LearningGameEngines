#pragma once

#include "Engine/Core/Base.h"
#include "Engine/Renderer/Texture.h"
#include "Engine/Scene/SceneCamera.h"
#include "Engine/Core/TimeStep.h"
#include "Engine/Core/UUID.h"

#include <glm/gtx/quaternion.hpp>

namespace Engine {

    struct IDComponent {
        UUID ID;
    };

    struct TagComponent {
        std::string Tag;
    };

    struct TransformComponent {
        glm::vec3 Translation{0.0f};
        glm::vec3 Rotation{0.0f};
        glm::vec3 Scale{1.0f};

        glm::mat4 GetTransform() const {
            glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

            return glm::translate(glm::mat4(1.0f), Translation)
                * rotation
                * glm::scale(glm::mat4(1.0f), Scale);
        }
    };

    struct SpriteRendererComponent {
        glm::vec4 Color{1.0f};
        Ref<Texture2D> Texture;
        float TilingFactor = 1.0f;
    };

    struct CircleRendererComponent {
        glm::vec4 Color{1.0f};
        float Thickness = 1.0f;
        float Fade = 0.005f;
    };

    struct CameraComponent {
        SceneCamera Camera;
        bool Primary = true;
        bool FixedAspectRatio = false;
    };

    class ScriptableEntity;

    struct NativeScriptComponent {
        ScriptableEntity* Instance = nullptr;

        ScriptableEntity* (* InstantiateFunction)();
        void (* DestroyInstantiateFunction)(NativeScriptComponent*);

        template<typename T>
        void Bind() {
            InstantiateFunction = []() { return static_cast<ScriptableEntity*>(new T()); };
            DestroyInstantiateFunction = [](NativeScriptComponent* nsc) {
                delete nsc->Instance;
                nsc->Instance = nullptr;
            };

        }

    };

    struct Rigidbody2DComponent {
        enum class BodyType { Static, Dynamic, Kinematic };
        BodyType Type = BodyType::Static;
        bool FixedRotation = false;

        void* RuntimeBody = nullptr;
    };

    struct BoxCollider2DComponent {
        glm::vec2 Offset{0.0f};
        glm::vec2 Size{0.5f};

        float Density = 1.0f;
        float Friction = 0.5f;
        float Restitution = 0.0f;
        float RestitutionThreshold = 0.5f;

        void* RuntimeFixture = nullptr;
    };

    struct CircleCollider2DComponent {
        glm::vec2 Offset{0.0f};
        float Radius = 0.5;

        float Density = 1.0f;
        float Friction = 0.5f;
        float Restitution = 0.0f;
        float RestitutionThreshold = 0.5f;

        void* RuntimeFixture = nullptr;
    };

} // Engine