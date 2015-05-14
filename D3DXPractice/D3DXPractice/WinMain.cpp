#include "pch.h"
#include "BoxApp.h"
#include "Application.h"


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
#if defined(DEBUT) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	//BoxApp theBox(hInstance);
	//if (!theBox.init())
	//	return false;

	//return theBox.run();

	Application myApplication;

	myApplication.Init();

	return myApplication.Run();

}