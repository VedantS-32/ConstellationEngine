#pragma once

#include "Entity.h"

namespace CStell
{
	class ScriptableEntity
	{
	public:
		template<typename T>
		CSTELL_API T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

	protected:
		CSTELL_API virtual void OnCreate() {}
		CSTELL_API virtual void OnDestroy() {}
		CSTELL_API virtual void OnUpdate(Timestep ts) {}

	private:
		Entity m_Entity;
		friend class Scene;
	};
}