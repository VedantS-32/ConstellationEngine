#pragma once

#include "Cstellpch.h"

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace CStell
{
	
	class CSTELL_API Log {

	public:
		static void Init();

		inline static std::shared_ptr < spdlog::logger >& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr < spdlog::logger >& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

//Core log macros
#define CSTELL_CORE_TRACE(...)	::CStell::Log::GetCoreLogger()->trace(__VA_ARGS__);
#define CSTELL_CORE_INFO(...)	::CStell::Log::GetCoreLogger()->info(__VA_ARGS__);
#define CSTELL_CORE_WARN(...)	::CStell::Log::GetCoreLogger()->warn(__VA_ARGS__);
#define CSTELL_CORE_ERROR(...)	::CStell::Log::GetCoreLogger()->error(__VA_ARGS__);
#define CSTELL_CORE_FATAL(...)	::CStell::Log::GetCoreLogger()->fatal(__VA_ARGS__);

//Client log macros
#define CSTELL_TRACE(...)	::CStell::Log::GetClientLogger()->trace(__VA_ARGS__);
#define CSTELL_INFO(...)	::CStell::Log::GetClientLogger()->info(__VA_ARGS__);
#define CSTELL_WARN(...)	::CStell::Log::GetClientLogger()->warn(__VA_ARGS__);
#define CSTELL_ERROR(...)	::CStell::Log::GetClientLogger()->error(__VA_ARGS__);
#define CSTELL_FATAL(...)	::CStell::Log::GetClientLogger()->fatal(__VA_ARGS__);