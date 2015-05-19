#pragma once

class Application
{
public:
	
	

	static Application* GetInstance();
	bool				Init();
	HWND                GetWndHandle() const;

	int					GetWndWidth() const;
	int					GetWndHeight() const;


	int					Run();


		
private:
	Application();
	~Application();

	bool	  CreateMyWindow();

private:
	HWND				m_hMainWnd;
	HINSTANCE			m_hInstance;
	int					m_WndWidth = 800;
	int					m_WndHeight = 600;
	TCHAR*				m_Title = nullptr;

};

