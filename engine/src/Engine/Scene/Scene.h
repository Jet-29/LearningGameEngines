#pragma once

#include "Engine/Core/TimeStep.h"
#include "Engine/Renderer/EditorCamera.h"
#include "Engine/Core/UUID.h"

#include <entt.hpp>

class b2World;

namespace Engine {
    class Entity;

    class Scene {
    public:
        Scene();
        ~Scene();

        static Ref<Scene> Copy(const Ref<Scene>& other);

        Entity CreateEntity(const std::string& name = std::string());
        Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
        void DestroyEntity(Entity entity);

        void OnRuntimeStart();
        void OnRuntimeStop();

        void OnSimulationStart();
        void OnSimulationStop();

        void OnUpdateEditor(TimeStep dt, EditorCamera& camera);
        void OnUpdateRuntime(TimeStep dt);
        void OnUpdateSimulation(TimeStep dt, EditorCamera& camera);
        void OnViewportResize(uint32_t width, uint32_t height);

        void DuplicateEntity(Entity entity);

        Entity GetPrimaryCameraEntity();

        template<typename... Components>
        auto GetAllEntitiesWith() {
            return m_Registry.view<Components...>();
        }

    private:
        template<typename T>
        void OnComponentAdded(Entity entity, T& component);

        void OnPhysics2DStart();
        void OnPhysics2DStop();

        void RenderScene(EditorCamera& camera);

        entt::registry m_Registry;
        uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

        b2World* m_PhysicsWorld = nullptr;

        friend class Entity;
        friend class SceneSerializer;
        friend class SceneHierarchyPanel;
    };
} // Engine