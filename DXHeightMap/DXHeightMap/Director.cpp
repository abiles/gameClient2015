#include "pch.h"
#include "Director.h"
#include "Renderer.h"

Director* Director::m_Instance = nullptr;

Director::Director()
{
	m_pRenderer = new Renderer();

}


Director::~Director()
{
	if (m_pRenderer)
		delete m_pRenderer;
	m_pRenderer = nullptr;
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

void Director::gameLoop()
{
	/// 여기서 작업을 한다. 
}
