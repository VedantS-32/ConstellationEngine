#pragma once

#include "CStell/Core/Core.h"
#include "CStell/Events/Event.h"

#include "CStell/Core/Timestep.h"

#include <imgui.h>

namespace CStell
{
	class CSTELL_API Layer
	{
	public:
		Layer(const std::string& debugName = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& e) {}

		inline const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};
}

