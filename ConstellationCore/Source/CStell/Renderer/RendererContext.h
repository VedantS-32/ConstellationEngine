#pragma once

namespace CStell
{
	class CSTELL_API RendererContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffer() = 0;
	};
}
