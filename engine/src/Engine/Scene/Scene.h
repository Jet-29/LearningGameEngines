#pragma once

#include <entt.hpp>
#include "Engine/Core/TimeStep.h"
#include "Engine/Scene/Components.h"

namespace Engine {
    class Entity;

    class Scene {
    public:
        Scene();
        ~Scene();

        Entity CreateEntity(const std::string& name = std::string());

        void OnUpdate(TimeStep dt);

    private:
        entt::registry m_Registry;

        friend class Entity;
    };

} // Engine