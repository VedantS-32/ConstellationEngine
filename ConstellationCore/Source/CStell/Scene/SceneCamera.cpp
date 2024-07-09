#include "CStellpch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace CStell
{
	SceneCamera::SceneCamera()
	{
		UpdateProjectionMatrix();
	}

	void SceneCamera::OnUpdate(Timestep ts)
	{
	}

	void SceneCamera::OnEvent(Event& e)
	{
	}

}
