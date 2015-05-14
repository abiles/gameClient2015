#pragma once

class Application
{
public:
	Application();
	~Application();
	

	static Application* GetInstance();
	bool				Init();
	HWND                GetWndHandle() const;

	int					GetWndWidth() const;
	int					GetWndHeight() const;


	int					Run();


		
private:
	bool	  CreateMyWindow();

	static Application* m_Instance;
	HWND				m_hMainWnd;
	HINSTANCE			m_hInstance;
	int					m_WndWidth = 800;
	int					m_WndHeight = 600;
	TCHAR*				m_Title = nullptr;

};

