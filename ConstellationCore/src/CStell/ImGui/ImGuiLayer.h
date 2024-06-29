#pragma once

#include "CStell/Core/Layer.h"

#include "CStell/Events/ApplicationEvent.h"
#include "CStell/Events/KeyEvent.h"
#include "CStell/Events/MouseEvent.h"

namespace CStell
{
	class ImGuiLayer : public Layer
	{
	public:
		CSTELL_API ImGuiLayer();
		CSTELL_API ~ImGuiLayer();

		CSTELL_API virtual void OnAttach() override;
		CSTELL_API virtual void OnDetach() override;
		CSTELL_API virtual void OnEvent(Event& e) override;
		CSTELL_API virtual void OnImGuiRender() override;

		CSTELL_API ImGuiContext& GetImGuiContext();

		CSTELL_API void Begin();
		CSTELL_API void End();

		CSTELL_API void BlockEvents(bool block) { m_BlockEvents = block; }

		CSTELL_API void SetDarkThemeColors();

	private:
		ImGuiContext* m_ImGuiContext;
		bool m_BlockEvents = true;
		float m_Time = 0.0f;
	};
}