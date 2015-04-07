#pragma once


class WindowManager
{
public:
	WindowManager();
	~WindowManager();

	static WindowManager*  getInstance();
	bool initWindow(TCHAR* title, int width, int height);

	HWND				 getHWND() const;
	
	bool                 run();


private:
	HINSTANCE     m_hInstanceHandle = NULL;

	int        m_MaxWidth = 800;
	int        m_MaxHeight = 600;

	TCHAR*     m_Title = nullptr;

	HWND       m_WndHandle = NULL;

};

