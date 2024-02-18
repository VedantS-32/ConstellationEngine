#pragma once

namespace CStell
{
	class RendererContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffer() = 0;
	};
}
