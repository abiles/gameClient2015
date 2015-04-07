#pragma once
#include "struct.h"


class WindowManager
{
public:
	WindowManager();
	~WindowManager();
	

	static WindowManager*  getInstance();
	bool				   initWindow(TCHAR* title, int width, int height);
	void				   showWindow(int nCmdShow);

	HWND				   getHWND() const;
	MySize				   getWndSize() const;
	
	bool                   run();


private:
	HINSTANCE     m_hInstanceHandle = NULL;
	HWND          m_WndHandle = NULL;
	TCHAR*        m_Title = nullptr;

	MySize	      m_WndSize;

};

