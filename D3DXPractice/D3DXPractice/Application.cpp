#include "pch.h"
#include "Application.h"
#include "Director.h"
#include "Triangle.h"
#include "HeightMap.h"


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return Director::GetInstance()->WndProc(hwnd, msg, wParam, lParam);
}

Application::Application()
{

}


Application::~Application()
{
}

bool Application::CreateMyWindow()
{
	m_hInstance = GetModuleHandle(nullptr);

	WNDCLASS WndClass;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = m_hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = m_Title;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	m_hMainWnd = CreateWindow(m_Title, m_Title, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		m_WndWidth, m_WndHeight, NULL, (HMENU)NULL, m_hInstance, NULL);

	if (m_hMainWnd == NULL)
	{
		return false;
	}
	else
	{
		return true;
	}

	return true;
}

bool Application::Init()
{
	m_Title = _T("HeightMap");

	if (!CreateMyWindow())
		return false;

	ShowWindow(m_hMainWnd, SW_SHOW);

	return true;
}

Application* Application::GetInstance()
{
	static Application instance;
	
	return &instance;
}

HWND Application::GetWndHandle() const
{
	return m_hMainWnd;
}

int Application::GetWndWidth() const
{
	return m_WndWidth;
}

int Application::GetWndHeight() const
{
	return m_WndHeight;
}

int Application::Run()
{
	MSG message;
	
	//Triangle triangle;
	//triangle.Init();

	HeightMap heightMap;
	heightMap.Init();

	
	while (true)
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			if (message.message == WM_QUIT)
			{
				break;
			}

			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			Director::GetInstance()->GameLoop(heightMap);
		}
	}

	return 0;
}
