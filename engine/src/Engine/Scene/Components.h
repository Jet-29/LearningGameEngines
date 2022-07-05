#pragma once

#include "Engine/Core/Base.h"
#include "Engine/Renderer/Texture.h"
#include "Engine/Scene/SceneCamera.h"
#include "Engine/Core/TimeStep.h"
#include "ScriptableEntity.h"

#include <glm/gtx/quaternion.hpp>

namespace Engine {

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

} // Engine