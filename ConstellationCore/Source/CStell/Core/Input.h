#pragma once
#include "CStell/Core/Core.h"
#include "CStell/Core/KeyCodes.h"
#include "CStell/Core/MouseButtonCodes.h"
#include "CStell/Core/InputMode.h"

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

		static void SetInputMode(Mode mode, Cursor value);
	};
}