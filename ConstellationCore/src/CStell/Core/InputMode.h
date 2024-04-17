#pragma once
#include <cstdint>

namespace CStell
{
	using Mode = uint32_t;
	using Cursor = uint32_t;

	namespace InputMode
	{
		enum : Mode
		{
			// From glfw3.h
			CSETLL_CURSOR               = 0x00033001,
			CSETLL_STICKY_KEYS          = 0x00033002,
			CSETLL_STICKY_MOUSE_BUTTONS = 0x00033003,
			CSETLL_LOCK_KEY_MODS        = 0x00033004,
			CSETLL_RAW_MOUSE_MOTION     = 0x00033005
		};
	}

	namespace CursorMode
	{
		enum : Cursor
		{
			CSTELL_CURSOR_NORMAL = 0x00034001,
			CSTELL_CURSOR_HIDDEN = 0x00034002,
			CSTELL_CURSOR_DISABLED = 0x00034003,
			CSTELL_CURSOR_CAPTURED = 0x00034004
		};
	}
}