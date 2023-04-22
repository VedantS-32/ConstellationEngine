#pragma once

#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"

namespace Cstell {
	
	class CSTELL_API Log {

	public:
		static void Init();

		inline static std::shared_ptr < spdlog::logger >& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr < spdlog::logger >& GetClientLogger() { return s_CoreLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

//Core log macros
#define CSTELL_CORE_TRACE(...)	::Cstell::Log::GetCoreLogger()->trace(__VA_ARGS__);
#define CSTELL_CORE_INFO(...)	::Cstell::Log::GetCoreLogger()->info(__VA_ARGS__);
#define CSTELL_CORE_WARN(...)	::Cstell::Log::GetCoreLogger()->warn(__VA_ARGS__);
#define CSTELL_CORE_ERROR(...)	::Cstell::Log::GetCoreLogger()->error(__VA_ARGS__);
#define CSTELL_CORE_FATAL(...)	::Cstell::Log::GetCoreLogger()->fatal(__VA_ARGS__);

//Client log macros
#define CSTELL_TRACE(...)	::Cstell::Log::GetCoreLogger()->trace(__VA_ARGS__);
#define CSTELL_INFO(...)	::Cstell::Log::GetCoreLogger()->info(__VA_ARGS__);
#define CSTELL_WARN(...)	::Cstell::Log::GetCoreLogger()->warn(__VA_ARGS__);
#define CSTELL_ERROR(...)	::Cstell::Log::GetCoreLogger()->error(__VA_ARGS__);
#define CSTELL_FATAL(...)	::Cstell::Log::GetCoreLogger()->fatal(__VA_ARGS__);