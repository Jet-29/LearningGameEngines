#pragma once

#include "Engine/Core/Base.h"
#include "Engine/Scene/Scene.h"
#include "Engine/Scene/Entity.h"

namespace Engine {

    class SceneHierarchyPanel {
    public:
        SceneHierarchyPanel() = default;
        SceneHierarchyPanel(const Ref<Scene>& context);

        void SetContext(const Ref<Scene>& context);

        void OnImGuiRender();

    private:
        void DrawEntityNode(Entity entity);
        void DrawComponents(Entity entity);

        Ref<Scene> m_Context;
        Entity m_SelectionContext;
    };
} // Engine