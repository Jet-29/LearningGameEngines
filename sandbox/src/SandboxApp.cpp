#include <Engine.h>

#include "imgui.h"

class ExampleLayer : public Engine::Layer {
public:
    ExampleLayer() : Layer("Example") {}

    void OnUpdate() override {
//		ENGINE_TRACE("ExampleLayer::OnUpdate");
    }

    virtual void OnImGuiRender() override {

    }

    void OnEvent(Engine::Event &event) override {
        if (event.GetEventType() == Engine::EventType::MouseMoved) {
            return;
        }
        ENGINE_INFO("{0}", event);
    }
};

class Sandbox : public Engine::Application {
public:
    Sandbox() {
        PushLayer(new ExampleLayer());
    }

    ~Sandbox() {

    }

};

Engine::Application *Engine::CreateApplication() {
    return new Sandbox();
}