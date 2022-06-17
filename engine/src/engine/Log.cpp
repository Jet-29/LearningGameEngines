#include "Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace Engine {

	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init() {
		spdlog::sink_ptr logSink;
		logSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		logSink->set_pattern("[%H:%M:%S:%f] [%n] [%^%l%$] %v");

		s_CoreLogger = std::make_shared<spdlog::logger>("ENGINE CORE", logSink);
		spdlog::register_logger(s_CoreLogger);
		s_CoreLogger->set_level(spdlog::level::trace);
		s_CoreLogger->flush_on(spdlog::level::trace);

		s_ClientLogger = std::make_shared<spdlog::logger>("ENGINE", logSink);
		spdlog::register_logger(s_ClientLogger);
		s_ClientLogger->set_level(spdlog::level::trace);
		s_ClientLogger->flush_on(spdlog::level::trace);

		ENGINE_CORE_INFO("Successfully initialized the logging system");
	}
} // Engine