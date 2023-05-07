#pragma once

#include "Core.h"


namespace Cstell {
	
	class CSTELL_API Application {

	public:
	
		Application();
		virtual ~Application();

		void Run();

	};
	
	//To be defined in CLIENT
	Application* CreateApplication();
}


