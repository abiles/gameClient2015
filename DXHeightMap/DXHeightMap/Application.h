#pragma once
class Application
{
public:
	Application();
	~Application();

	bool CreateMyWindow();



private:
	HINSTANCE m_hAppInst = NULL;
	HWND      m_hMainWnd = NULL;
};

