#pragma once

#include "Core.h"

namespace Engine {
	class ENGINE_API Application {
	public:
		Application() = default;
		virtual ~Application() = default;

		void Run();
	};

	// to define in client
	Application *CreateApplication();

} // Engine