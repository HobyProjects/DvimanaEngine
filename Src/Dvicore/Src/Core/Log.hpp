#pragma once

#include <memory>

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace Dvimana {
	class Log {
		public:
			static bool Init();
			static std::shared_ptr<spdlog::logger>& GetCoreLogger();
			static std::shared_ptr<spdlog::logger>& GetClientLogger();
	};
}

#define DVI_CORE_TRACE(...)    ::Dvimana::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define DVI_CORE_INFO(...)     ::Dvimana::Log::GetCoreLogger()->info(__VA_ARGS__)
#define DVI_CORE_WARN(...)     ::Dvimana::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define DVI_CORE_ERROR(...)    ::Dvimana::Log::GetCoreLogger()->error(__VA_ARGS__)
#define DVI_CORE_CRITICAL(...) ::Dvimana::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define DVI_TRACE(...)         ::Dvimana::Log::GetClientLogger()->trace(__VA_ARGS__)
#define DVI_INFO(...)          ::Dvimana::Log::GetClientLogger()->info(__VA_ARGS__)
#define DVI_WARN(...)          ::Dvimana::Log::GetClientLogger()->warn(__VA_ARGS__)
#define DVI_ERROR(...)         ::Dvimana::Log::GetClientLogger()->error(__VA_ARGS__)
#define DVI_CRITICAL(...)      ::Dvimana::Log::GetClientLogger()->critical(__VA_ARGS__)