#pragma once

int main(int argc, char **argv) {

	Engine::Log::Init();

	auto app = Engine::CreateApplication();
	app->Run();
	delete app;
}