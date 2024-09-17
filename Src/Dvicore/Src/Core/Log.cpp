#include "Log.hpp"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace DviCore 
{
    static bool s_Initialized{false};
    static std::shared_ptr<spdlog::logger> s_CoreLogger{nullptr};
    static std::shared_ptr<spdlog::logger> s_ClientLogger{nullptr};

	void Log::Init() 
    {
        std::vector<spdlog::sink_ptr> log_skin{};
        log_skin.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
        log_skin.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Dvimana.log", true));
        log_skin[0]->set_pattern("%^[%T] %n: %v%$");
        log_skin[1]->set_pattern("[%T][%l] %n: %v");

        s_CoreLogger = std::make_shared<spdlog::logger>("Dvimana-Core", std::begin(log_skin), std::end(log_skin));
        spdlog::register_logger(s_CoreLogger);
        s_CoreLogger->set_level(spdlog::level::trace);
        s_CoreLogger->flush_on(spdlog::level::trace);

        s_ClientLogger = std::make_shared<spdlog::logger>("Dvimana-Client", std::begin(log_skin), std::end(log_skin));
        spdlog::register_logger(s_ClientLogger);
        s_ClientLogger->set_level(spdlog::level::trace);
        s_ClientLogger->flush_on(spdlog::level::trace);
        s_Initialized = true;
	}

	std::shared_ptr<spdlog::logger>& Log::GetCoreLogger() {
        if(!s_Initialized)
            Init();

		return s_CoreLogger;
	}

	std::shared_ptr<spdlog::logger>& Log::GetClientLogger() {
        if(!s_Initialized)
            Init();
            
		return s_ClientLogger;
	}
}