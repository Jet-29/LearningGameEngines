#include "EnginePch.h"
#include "Scene.h"
#include "Engine/Renderer/Renderer2D.h"

#include "Entity.h"

namespace Engine {
    Scene::Scene() {

    }
    Scene::~Scene() {

    }
    Entity Scene::CreateEntity(const std::string& name) {
        Entity entity = {m_Registry.create(), this};
        entity.AddComponent<TransformComponent>();
        auto& tag = entity.AddComponent<TagComponent>();
        tag.Tag = name.empty() ? "Entity" : name;
        return entity;
    }
    void Scene::OnUpdate(TimeStep dt) {
        Camera* mainCamera = nullptr;
        glm::mat4* cameraTransform = nullptr;
        auto cameraGroup = m_Registry.view<TransformComponent, CameraComponent>();
        for (auto entity : cameraGroup) {
            auto [transform, camera] = cameraGroup.get<TransformComponent, CameraComponent>(entity);
            if (camera.Primary) {
                mainCamera = &camera.Camera;
                cameraTransform = &transform.Transform;
                break;
            }
        }

        if (mainCamera) {
            Renderer2D::BeginScene(*mainCamera, *cameraTransform);
            auto spriteGroup = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
            for (auto entity : spriteGroup) {
                auto [transform, sprite] = spriteGroup.get<TransformComponent, SpriteRendererComponent>(entity);
                Renderer2D::DrawQuad(transform.Transform, sprite.Color);
            }
            Renderer2D::EndScene();
        }
    }
} // Engine