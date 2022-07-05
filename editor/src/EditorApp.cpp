#include <Engine.h>
#include <Engine/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Engine {
    class EditorApp : public Application {
    public:
        EditorApp(const ApplicationSpecification& spec) : Application(spec) {
            PushLayer(new EditorLayer());
        }
    };

    Application* CreateApplication(ApplicationCommandLineArgs args) {
        ApplicationSpecification spec;
        spec.Name = "Hazelnut";
        spec.CommandLineArgs = args;

        return new EditorApp(spec);
    }
}