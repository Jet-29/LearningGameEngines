#pragma once

#include "Engine/Core/Base.h"
#include "Engine/Events/Event.h"
#include "TimeStep.h"

namespace Engine {

    class ENGINE_API Layer {
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(TimeStep dt) {}
        virtual void OnImGuiRender() {}
        virtual void OnEvent(Event& event) {}

        inline std::string GetName() const { return m_DebugName; }

    protected:
        std::string m_DebugName;
    };

} // Engine