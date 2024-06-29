#pragma once

#include "CStell/Core/Core.h"
#include "CStell/Events/Event.h"

#include "CStell/Core/Timestep.h"

#include <imgui.h>

namespace CStell
{
	class Layer
	{
	public:
		CSTELL_API Layer(const std::string& debugName = "Layer");
		CSTELL_API virtual ~Layer();

		CSTELL_API virtual void OnAttach() {}
		CSTELL_API virtual void OnDetach() {}
		CSTELL_API virtual void OnUpdate(Timestep ts) {}
		CSTELL_API virtual void OnImGuiRender() {}
		CSTELL_API virtual void OnEvent(Event& e) {}

		inline const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};
}

