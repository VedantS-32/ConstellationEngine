#pragma once

#include "CStell/Core/Core.h"
#include "Layer.h"

#include <vector>

namespace CStell
{
	class LayerStack
	{
	public:
		CSTELL_API LayerStack();
		CSTELL_API ~LayerStack();

		CSTELL_API void PushLayer(Layer* layer);
		CSTELL_API void PushOverlay(Layer* overlay);
		CSTELL_API void PopLayer(Layer* layer);
		CSTELL_API void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
		std::vector<Layer*>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
		std::vector<Layer*>::reverse_iterator rend() { return m_Layers.rend(); }

		std::vector<Layer*>::const_iterator begin() const { return m_Layers.begin(); }
		std::vector<Layer*>::const_iterator end()	const { return m_Layers.end(); }
		std::vector<Layer*>::const_reverse_iterator rbegin() const { return m_Layers.rbegin(); }
		std::vector<Layer*>::const_reverse_iterator rend() const { return m_Layers.rend(); }

	private:
		std::vector<Layer*> m_Layers;
		unsigned int m_LayerInsertIndex = 0;
	};
}

