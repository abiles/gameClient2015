#include "pch.h"
#include "WindowManager.h"



int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   	 LPSTR lpszCmdParam, int nCmdShow)
{
	WindowManager::getInstance()->initWindow(_T("MyApp"), 800, 600);
	WindowManager::getInstance()->showWindow(nCmdShow);



	WindowManager::getInstance()->run();
	
	return 0;
}