#pragma once
class Application
{
public:
	Application();
	~Application();

	bool CreateMyWindow();
	static Application* GetInstance();
	int					Run();


private:
	static Application* m_Instance;
	HINSTANCE m_hAppInst = NULL;
	HWND      m_hMainWnd = NULL;
};

