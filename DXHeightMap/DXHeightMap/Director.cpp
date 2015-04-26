#include "pch.h"
#include "Director.h"

Director* Director::m_Instance = nullptr;

Director::Director()
{
}


Director::~Director()
{
}

Director* Director::GetInstance()
{
	if (!m_Instance)
	{
		m_Instance = new Director();
	}

	return m_Instance;
}

LRESULT CALLBACK Director::WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return true;
}
