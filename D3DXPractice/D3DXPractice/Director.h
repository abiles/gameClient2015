#pragma once


#define GET_DIRECTOR Director::GetInstance


class Camera;
class KeyStateManager;
class Layer;

class Director
{
public:
	Director();
	~Director();

	static Director*	GetInstance();

	LRESULT CALLBACK	WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
	void				GameLoop(Layer& layer);
	void                CameraWalkByKeyState();
	void				CameraRotateByMouse();
	Camera*				GetCamera() const;

private:

	KeyStateManager* m_KeyStateManager = nullptr;
	Camera* m_Camera = nullptr;
	

};

