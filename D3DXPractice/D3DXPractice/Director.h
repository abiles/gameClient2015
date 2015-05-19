#pragma once


class Camera;
class KeyStateManager;
class Node;

class Director
{
public:
	Director();
	~Director();

	static Director*	GetInstance();

	LRESULT CALLBACK	WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
	void				GameLoop(Node& node);
	void                CameraWalkByKeyState();

private:

	KeyStateManager* m_KeyStateManager = nullptr;
	Camera* m_Camera = nullptr;

};

