#include <CStell.h>
#include <CStellpch.h>

class Sandbox : public CStell::Application
{

public:
	Sandbox() {

	}

	~Sandbox() {

	}

};


CStell::Application* CStell::CreateApplication()
{

	return new Sandbox();
}