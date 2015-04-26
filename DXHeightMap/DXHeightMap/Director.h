#pragma once
class Director
{
public:
	Director();
	~Director();

	static Director* GetInstance();

	LRESULT CALLBACK	WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
private:
	static Director* m_Instance;
};



