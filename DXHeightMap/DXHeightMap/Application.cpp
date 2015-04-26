#include "pch.h"
#include "Application.h"
#include "Director.h"
#include "Info.h"



Application::Application()
{
}


Application::~Application()
{
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return Director::GetInstance()->WndProc(hWnd, iMessage, wParam, lParam);
}

bool Application::CreateMyWindow()
{
	WNDCLASS WndClass;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = m_hAppInst;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = L"HeightMap";
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);


	m_hMainWnd = CreateWindow(L"HeightMap", TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		WNDWIDTH, WNDHEIGHT, NULL, (HMENU)NULL, m_hAppInst, NULL);

	if (m_hMainWnd == NULL)
	{
		return false;
	}
	else
	{
		ShowWindow(m_hMainWnd, SW_SHOW);
		return true;
	}



	if (!m_hMainWnd)
	{
		DWORD err = GetLastError();
		TCHAR buffer[200] = { 0, };
		wsprintf(buffer, L"getlasterror : %d", err);
		MessageBox(0, buffer, 0, 0);

		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}


	UpdateWindow(m_hMainWnd);

	return true;
}


