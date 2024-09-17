#pragma once

#include <memory>

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace DviCore 
{
	class Log 
	{
		public:
			static void Init();
			static std::shared_ptr<spdlog::logger>& GetCoreLogger();
			static std::shared_ptr<spdlog::logger>& GetClientLogger();
	};
}

#define DVI_CORE_TRACE(...)    ::DviCore::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define DVI_CORE_INFO(...)     ::DviCore::Log::GetCoreLogger()->info(__VA_ARGS__)
#define DVI_CORE_WARN(...)     ::DviCore::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define DVI_CORE_ERROR(...)    ::DviCore::Log::GetCoreLogger()->error(__VA_ARGS__)
#define DVI_CORE_CRITICAL(...) ::DviCore::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define DVI_TRACE(...)         ::DviCore::Log::GetClientLogger()->trace(__VA_ARGS__)
#define DVI_INFO(...)          ::DviCore::Log::GetClientLogger()->info(__VA_ARGS__)
#define DVI_WARN(...)          ::DviCore::Log::GetClientLogger()->warn(__VA_ARGS__)
#define DVI_ERROR(...)         ::DviCore::Log::GetClientLogger()->error(__VA_ARGS__)
#define DVI_CRITICAL(...)      ::DviCore::Log::GetClientLogger()->critical(__VA_ARGS__)