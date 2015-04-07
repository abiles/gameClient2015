#include "pch.h"
#include "WindowManager.h"
#include "DeviceManager.h"



int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   	 LPSTR lpszCmdParam, int nCmdShow)
{
	WindowManager::getInstance()->initWindow(_T("MyApp"), 800, 600);
	WindowManager::getInstance()->showWindow(nCmdShow);
	HRESULT hr = DeviceManager::getInstance()->initDevice();

	if (FAILED(hr))
		return -1;

	WindowManager::getInstance()->run();
	
	return 0;
}