#pragma once

#include "CStell/Layer.h"

#include "CStell/Events/ApplicationEvent.h"
#include "CStell/Events/KeyEvent.h"
#include "CStell/Events/MouseEvent.h"

namespace CStell
{
	class CSTELL_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

	private:
		float m_Time = 0.0f;
	};
}