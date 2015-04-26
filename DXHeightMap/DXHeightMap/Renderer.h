#pragma once
class Renderer
{
public:
	Renderer();
	~Renderer();

	bool Init();

private:
	void SwapChainInit();

private:
	
	ID3D11Device*			 m_pDevice = nullptr;
	ID3D11DeviceContext*	 m_pImmediateContext = nullptr;
	IDXGISwapChain*			 m_pSwapChain = nullptr;
	D3D_FEATURE_LEVEL        m_FeatureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11Texture2D*		 m_pDepthStencilBuffer = nullptr;
	ID3D11DepthStencilView*  m_pDepthStencilView = nullptr;
	ID3D11RenderTargetView*  m_pRenderTargetView = nullptr;
	D3D11_VIEWPORT			 m_ScreenViewPort;

	std::wstring m_MainWndTitle = L"DXHeightMap";

	D3D_DRIVER_TYPE m_D3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
};
