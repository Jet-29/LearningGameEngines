#pragma once

#include "Engine/Core/Base.h"
#include "Engine/Core/Application.h"

extern Engine::Application* Engine::CreateApplication(ApplicationCommandLineArgs args);

int main(int argc, char** argv) {

    Engine::Log::Init();

    auto app = Engine::CreateApplication({argc, argv});
    app->Run();
    delete app;
}