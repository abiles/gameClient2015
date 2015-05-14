#pragma once


class Camera;
class KeyStateManager;
class Director
{
public:
	Director();
	~Director();

	static Director*	GetInstance();

	LRESULT CALLBACK	WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
	void				GameLoop();
	void                CameraWalkByKeyState();

private:

	KeyStateManager* m_KeyStateManager = nullptr;
	Camera* m_Camera = nullptr;

};

