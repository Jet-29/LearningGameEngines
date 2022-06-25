#include <Engine.h>
#include <Engine/Core/EntryPoint.h>

#include "Layers/ExampleLayer.h"
#include "Layers/Example2D.h"

class Sandbox : public Engine::Application {
public:
    Sandbox() {
//        PushLayer(new ExampleLayer());
        PushLayer(new Example2D());
    }
};

Engine::Application *Engine::CreateApplication() {
    return new Sandbox();
}