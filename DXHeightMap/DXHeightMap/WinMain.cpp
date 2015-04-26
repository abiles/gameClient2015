#include "pch.h"
#include "Application.h"


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
#if defined(DEBUT) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	Application myApplication(hInstance);
	myApplication.CreateMyWindow();

	return myApplication.Run();
}