#include "pch.h"
#include "Director.h"
#include "Renderer.h"
#include "Shader.h"
#include "Camera.h"
#include "KeyStateManager.h"
#include "Node.h"
#include "Layer.h"


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
	
		m_KeyStateManager->SetMouseX(LOWORD(lParam));
		m_KeyStateManager->SetMouseY(HIWORD(lParam));
		return 0;
	case WM_LBUTTONDOWN:
		m_KeyStateManager->SetLButtonDown();
		return 0;
	case WM_LBUTTONUP:
		m_KeyStateManager->SetLButtonUp();
		return 0;
	case WM_RBUTTONDOWN:
		m_KeyStateManager->SetRButtonDown();
		return 0;
	case WM_RBUTTONUP:
		m_KeyStateManager->SetRButtonUp();
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

void Director::GameLoop(Layer& layer)
{

	m_KeyStateManager->KeyInput();
	CameraWalkByKeyState();
	CameraRotateByMouse();

	GET_RENDERER()->Render(layer);
}

void Director::CameraWalkByKeyState()
{
	BYTE* CurrentKey = m_KeyStateManager->GetCurrentKey();

	float moveSize = 0.05f;
	if (CurrentKey['W'] & HOLDKEY)
	{
		m_Camera->Walk(moveSize);
	}
	else if (CurrentKey['S'] & HOLDKEY)
	{
		m_Camera->Walk(-moveSize);

	}
	else if (CurrentKey['A'] & HOLDKEY)
	{
		m_Camera->Strafe(-moveSize);


	}
	else if (CurrentKey['D'] & HOLDKEY)
	{
		m_Camera->Strafe(moveSize);

	}

	m_Camera->UpdateViewMatrix();

	
}

Camera* Director::GetCamera() const
{
	return m_Camera;
}

void Director::CameraRotateByMouse()
{
	if (m_KeyStateManager->IsLButtonClick())
	{
		float oldX = m_KeyStateManager->GetOldMouseX();
		float oldY = m_KeyStateManager->GetOldMouseY();

		float x = m_KeyStateManager->GetMouseX();
		float y = m_KeyStateManager->GetMouseY();
		float dx = XMConvertToRadians(0.25f * (x - oldX));
		float dy = XMConvertToRadians(0.25f * (y - oldY));

		m_Camera->Pitch(dy);
		m_Camera->RotateY(dx);
	}

	m_KeyStateManager->SetOldMouseX();
	m_KeyStateManager->SetOldMouseY();
}


