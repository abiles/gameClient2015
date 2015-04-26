#pragma once
class Application
{
public:
	Application(HINSTANCE hInstance);
	~Application();

	static Application* GetInstance();
	bool				CreateMyWindow();
	int					Run();
	HWND				GetWindowHandle() const;


private:
	static Application* m_Instance;
	HINSTANCE m_WndInstanceHandle = NULL;
	HWND      m_hMainWnd = NULL;
};

