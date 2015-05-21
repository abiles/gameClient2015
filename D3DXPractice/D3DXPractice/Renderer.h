#pragma once

#define GET_DEVICE() Renderer::GetInstance()->GetDevice()
#define GET_RENDERER() Renderer::GetInstance()
#define GET_DEVICECONTEXT() Renderer::GetInstance()->GetDeviceContext() 

class Layer;
class Renderer
{
public:
	static Renderer*	GetInstance();
	void Render(Layer& node);

	ID3D11Device*		 GetDevice() const;
	ID3D11DeviceContext* GetDeviceContext() const;

private:
	Renderer();
	~Renderer();

	bool					 Init();
	void					 OnResize();

private:

	bool					 m_Enable4XMsaa = false;
	UINT					 m_4xMsaaQuality = 0;

	ID3D11Device*			 m_D3dDevice = nullptr;
	ID3D11DeviceContext*	 m_D3dImmediateContext = nullptr;
	IDXGISwapChain*			 m_SwapChain = nullptr;
	ID3D11Texture2D*		 m_DepthStencilBuffer = nullptr;
	ID3D11DepthStencilView*  m_DepthStencilView = nullptr;
	ID3D11RenderTargetView*  m_RenderTargetView = nullptr;
	D3D11_VIEWPORT			 m_ScreenViewPort;

	std::wstring m_MainWndTitle = L"D3DHeightMap";

	D3D_DRIVER_TYPE m_D3dDriverType = D3D_DRIVER_TYPE_HARDWARE;

private:
	float m_ClearColor[4];
};

