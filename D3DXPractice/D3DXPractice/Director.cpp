#include "pch.h"
#include "Director.h"
#include "Renderer.h"
#include "Shader.h"
#include "Camera.h"
#include "KeyStateManager.h"
#include "Node.h"


Director::Director()
{
	m_KeyStateManager = new KeyStateManager();
	m_Camera = new Camera();
}


Director::~Director()
{
	if (m_KeyStateManager)
		delete m_KeyStateManager;
	m_KeyStateManager = nullptr;

	if (m_Camera)
		delete m_Camera;
	m_Camera = nullptr;
}

Director* Director::GetInstance()
{

	static Director Singleton;

	return &Singleton;

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

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}

void Director::GameLoop(Node& node)
{
	//m_KeyStateManager->KeyInput();
	//CameraWalkByKeyState();

	//node.DrawByVertex();
	GET_RENDERER()->Render(node);
}

void Director::CameraWalkByKeyState()
{
	BYTE* CurrentKey = m_KeyStateManager->GetCurrentKey();

	if (CurrentKey['W'] & HOLDKEY)
	{
		m_Camera->Walk(10.0f);
	}
	else if (CurrentKey['S'] & HOLDKEY)
	{
		m_Camera->Walk(-10.0f);

	}

	if (CurrentKey['A'] & HOLDKEY)
	{
		m_Camera->Strafe(10.0f);

	}
	else if (CurrentKey['D'] & HOLDKEY)
	{
		m_Camera->Strafe(-10.0f);
	}

	m_Camera->UpdateViewMatrix();

	
}


