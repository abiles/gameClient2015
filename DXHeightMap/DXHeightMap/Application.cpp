#include "pch.h"
#include "Application.h"
#include "Director.h"
#include "WndInfo.h"
#include "Renderer.h"

Application* Application::m_Instance = nullptr;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return Director::GetInstance()->WndProc(hWnd, iMessage, wParam, lParam);
}

Application::Application(HINSTANCE hInstance)
{
	m_WndInstanceHandle = hInstance;
	
	m_Instance = this;
}


Application::~Application()
{
}


bool Application::CreateMyWindow()
{
	/*WNDCLASS WndClass;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = m_WndInstanceHandle;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = _T("HeightMap");
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	if (!RegisterClass(&WndClass))
	return false;
	*/
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_WndInstanceHandle;
	wcex.hIcon = LoadIcon(m_WndInstanceHandle, IDI_APPLICATION);
	wcex.hIconSm = LoadIcon(m_WndInstanceHandle, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = _T("HeightMap");

	if (!RegisterClassEx(&wcex))
		return false;


	/*m_hMainWnd = CreateWindow(_T("HeightMap"), TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		WNDWIDTH, WNDHEIGHT, NULL, (HMENU)NULL, m_WndInstanceHandle, NULL);*/

	m_hMainWnd = CreateWindowEx(WS_EX_APPWINDOW
		, _T("HeightMap")
		, _T("HeightMap")
		, WS_OVERLAPPEDWINDOW
		, CW_USEDEFAULT
		, CW_USEDEFAULT
		, 800
		, 600
		, NULL
		, NULL
		, m_WndInstanceHandle
		, NULL);

	if (!m_hMainWnd)
	{
		DWORD err = GetLastError();
		TCHAR buffer[200] = { 0, };
		wsprintf(buffer, L"getlasterror : %d", err);
		MessageBox(0, buffer, 0, 0);

		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}

	if (m_hMainWnd == NULL)
	{
		return false;
	}
	else
	{
		ShowWindow(m_hMainWnd, SW_SHOW);
		return true;
	}

	UpdateWindow(m_hMainWnd);

	return true;
}

Application* Application::GetInstance()
{
	return m_Instance;
}

int Application::Run()
{
	MSG message;

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
			Director::GetInstance()->GameLoop();
		}
	}

	return 0;

}

HWND Application::GetWindowHandle() const
{
	return m_hMainWnd;
}


