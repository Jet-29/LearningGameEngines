#include <Engine.h>
#include <Engine/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Engine {
    class EditorApp : public Application {
    public:
        EditorApp() : Application("Editor") {
            PushLayer(new EditorLayer());
        }
    };

    Application* CreateApplication() {
        return new EditorApp();
    }
}