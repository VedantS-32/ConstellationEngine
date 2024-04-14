#pragma once
#include "CStell/Core/Core.h"
#include "CStell/Core/KeyCodes.h"
#include "CStell/Core/MouseButtonCodes.h"

namespace CStell
{
	class CSTELL_API Input
	{
	public:
		static bool IsKeyPressed(const KeyCode keycode);

		static bool IsMouseButtonPressed(const MouseCode keycode);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}