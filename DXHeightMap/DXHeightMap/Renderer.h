#pragma once
class Renderer
{
public:
	Renderer();
	~Renderer();

private:
	ID3D11Device*			 m_D3dDevice = nullptr;
	ID3D11DeviceContext*	 m_D3dImmediateContext = nullptr;
	IDXGISwapChain*			 m_SwapChain = nullptr;
	ID3D11Texture2D*		 m_DepthStencilBuffer = nullptr;
	ID3D11DepthStencilView*  m_DepthStencilView = nullptr;
	ID3D11RenderTargetView*  m_RenderTargetView = nullptr;
	D3D11_VIEWPORT			 m_ScreenViewPort;

	std::wstring m_MainWndTitle = L"DXHeightMap";

	D3D_DRIVER_TYPE m_D3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
};

