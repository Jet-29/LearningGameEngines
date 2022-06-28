#include <Engine.h>
#include <Engine/Core/EntryPoint.h>

#include "Layers/ExampleLayer.h"
#include "Layers/Example2DLayer.h"
#include "Layers/ParticleSystemLayer.h"
#include "Layers/TileMapExampleLayer.h"

class Sandbox : public Engine::Application {
public:
    Sandbox() {
//        PushLayer(new ExampleLayer());
        PushLayer(new EditorLayer());
//        PushLayer(new ParticleSystemLayer());
//        PushLayer(new TileMapExampleLayer());
    }
};

Engine::Application* Engine::CreateApplication() {
    return new Sandbox();
}