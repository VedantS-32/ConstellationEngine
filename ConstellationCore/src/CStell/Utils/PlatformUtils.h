#pragma once

#include <string>

namespace CStell
{
	class FileDialogs
	{
	public:
		// Returns empty strings if cancelled
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
	};
}
