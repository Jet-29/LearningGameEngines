#pragma once

#include <entt.hpp>
#include "Engine/Core/TimeStep.h"
#include "Engine/Renderer/EditorCamera.h"

namespace Engine {
    class Entity;

    class Scene {
    public:
        Scene();
        ~Scene();

        Entity CreateEntity(const std::string& name = std::string());
        void DestroyEntity(Entity entity);

        void OnUpdateEditor(TimeStep dt, EditorCamera& camera);
        void OnUpdateRuntime(TimeStep dt);
        void OnViewportResize(uint32_t width, uint32_t height);

        Entity GetPrimaryCameraEntity();

    private:
        template<typename T>
        void OnComponentAdded(Entity entity, T& component);

        entt::registry m_Registry;
        uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

        friend class Entity;
        friend class SceneSerializer;
        friend class SceneHierarchyPanel;
    };
} // Engine