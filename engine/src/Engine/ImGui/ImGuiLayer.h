#pragma once

#include "Engine/Core/Layer.h"
#include "engine/Events/MouseEvent.h"
#include "engine/Events/KeyEvent.h"
#include "engine/Events/ApplicationEvent.h"

namespace Engine {

    class ImGuiLayer : public Layer {
    public:
        ImGuiLayer();
        ~ImGuiLayer() override;

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate() override;
        void OnEvent(Event &event) override;

    private:
        float m_Time = 0.0f;
    };

} // Engine