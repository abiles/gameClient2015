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
	HDC hdc;
	PAINTSTRUCT ps;

	switch (iMessage)
	{
	case WM_CREATE:
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_MOUSEMOVE:
		return 0;
	case WM_LBUTTONDOWN:
		return 0;
	case WM_LBUTTONUP:
		return 0;
	case WM_RBUTTONDOWN:
		return 0;
	case WM_RBUTTONUP:
		return 0;
	case WM_DESTROY:
		return 0;
	case WM_SIZE:
		return 0;
	case WM_EXITSIZEMOVE:
		return 0;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

void Director::GameLoop()
{
	/// 여기서 작업을 한다. 
	m_pRenderer->Render();
}
