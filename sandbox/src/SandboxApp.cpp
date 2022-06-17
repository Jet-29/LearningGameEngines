#include <Engine.h>

class ExampleLayer : public Engine::Layer {
public:
	ExampleLayer() : Layer("Example") {}

	void OnUpdate() override {
		ENGINE_TRACE("ExampleLayer::OnUpdate");
	}

	void OnEvent(Engine::Event &event) override {
		ENGINE_INFO("{0}", event);
	}
};

class Sandbox : public Engine::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
		PushOverlay(new Engine::ImGuiLayer());
	}

	~Sandbox() {

	}

};

Engine::Application *Engine::CreateApplication() {
	return new Sandbox();
}