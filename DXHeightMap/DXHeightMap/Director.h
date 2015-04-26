#pragma once

class Renderer;
class Director
{
public:
	Director();
	~Director();

	static Director* GetInstance();

	LRESULT CALLBACK	WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
	void gameLoop();
private:
	static Director* m_Instance;
	Renderer*		 m_pRenderer;
};



