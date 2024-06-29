#pragma once

#include "CStell/Core/Layer.h"

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
		virtual void OnEvent(Event& e) override;
		virtual void OnImGuiRender() override;

		ImGuiContext& GetImGuiContext();

		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block; }

		void SetDarkThemeColors();

	private:
		ImGuiContext* m_ImGuiContext;
		bool m_BlockEvents = true;
		float m_Time = 0.0f;
	};
}