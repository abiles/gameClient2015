#include "pch.h"
#include "WindowManager.h"



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC	hdc;
	PAINTSTRUCT	ps;

	switch (message)
	{
	case WM_CREATE:

		break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}


WindowManager::WindowManager()
{
		
}


WindowManager::~WindowManager()
{
}

WindowManager* WindowManager::getInstance()
{
	static WindowManager windowManager;
	
	return &windowManager;
}



bool WindowManager::initWindow(TCHAR* title, int width, int height)
{
	m_hInstanceHandle = GetModuleHandle(nullptr);
	m_Title = title;
	m_WndSize.m_MaxWidth = width;
	m_WndSize.m_MaxHeight = height;

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInstanceHandle;
	wcex.hIcon = LoadIcon(m_hInstanceHandle, IDI_APPLICATION);
	wcex.hIconSm = LoadIcon(m_hInstanceHandle, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = _T("MyClass");

	if (!RegisterClassEx(&wcex))
		return false;

	HWND	hWnd = CreateWindowEx(WS_EX_APPWINDOW
		, _T("MyClass")
		, m_Title
		, WS_OVERLAPPEDWINDOW
		, CW_USEDEFAULT
		, CW_USEDEFAULT
		, m_WndSize.m_MaxWidth
		, m_WndSize.m_MaxHeight
		, NULL
		, NULL
		, m_hInstanceHandle
		, NULL);

	m_WndHandle = hWnd;

	if (!m_WndHandle)
		return false;

	return true;
}

HWND WindowManager::getHWND() const
{
	return m_WndHandle;
}

bool WindowManager::run()
{
	MSG			msg;
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (WM_QUIT == msg.message)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
		

		}
	}

	return true;
}

void WindowManager::showWindow(int nCmdShow)
{
	ShowWindow(m_WndHandle, nCmdShow);
}

MySize WindowManager::getWndSize() const
{
	return m_WndSize;
}

