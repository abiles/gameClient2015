#pragma once



class Timer;
class D3DXApp
{
public:
	D3DXApp(HINSTANCE hInstace);
	virtual ~D3DXApp();

	HINSTANCE appInst() const;
	HWND	  mainWnd() const;
	float     aspectRatio() const;

	int run();

	virtual bool init();
	virtual void onResize();
	virtual void updateScene(float dt) = 0;
	virtual void drawScene() = 0;
	virtual LRESULT msgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	virtual void onMouseDown(WPARAM btnState, int x, int y);
	virtual void onMouseUp(WPARAM btnState, int x, int y);
	virtual void onMouseMove(WPARAM btnState, int x, int y);

protected:
	bool initMainWindow();
	bool initDirect3D();

	void calculateFrameStats();

protected:
	HINSTANCE m_hAppInst = NULL;
	HWND      m_hMainWnd = NULL;
	bool      m_AppPaused = false;
	bool      m_Minimized = false; 
	bool      m_Maximized = false;
	bool      m_Resizing = false;
	UINT      m_4xMsaaQuality = 0;

	Timer* m_Timer = nullptr;

	
	bool m_Enable4XMsaa = false;
	ID3D11Device*			 m_D3dDevice = nullptr;
	ID3D11DeviceContext*	 m_D3dImmediateContext = nullptr;
	IDXGISwapChain*			 m_SwapChain = nullptr;
	ID3D11Texture2D*		 m_DepthStencilBuffer = nullptr;
	ID3D11DepthStencilView*  m_DepthStencilView = nullptr;
	ID3D11RenderTargetView*  m_RenderTargetView = nullptr;
	D3D11_VIEWPORT			 m_ScreenViewPort;

	std::wstring m_MainWndTitle = L"D3DHeightMap";
	
	D3D_DRIVER_TYPE m_D3dDriverType = D3D_DRIVER_TYPE_HARDWARE;

	int m_ClientWidth = 800;
	int m_ClientHeight = 600;





};

