#pragma once

#include "Engine/Core/Layer.h"
#include "Engine/Events/MouseEvent.h"
#include "Engine/Events/KeyEvent.h"
#include "Engine/Events/ApplicationEvent.h"

namespace Engine {

    class ImGuiLayer : public Layer {
    public:
        ImGuiLayer();
        virtual ~ImGuiLayer() override;

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnEvent(Event& event) override;

        static void Begin();
        static void End();

        void BlockEvents(bool block) { m_BlockEvents = block; }

        static void SetDarkThemeColors();
    private:
        bool m_BlockEvents = false;
        float m_Time = 0.0f;
    };

} // Engine