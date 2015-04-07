#pragma once
#include "struct.h"

class DeviceManager
{
public:
	DeviceManager();
	~DeviceManager();

	HRESULT initDevice();
	HRESULT createSwapChain();
	HRESULT createDepthStencilTexture();
	HRESULT createShader();
	void setViewport();

private:
	IDXGISwapChain*				m_pSwapChain = nullptr;
	ID3D11Device*				m_pD3dDevice = nullptr;
	D3D_FEATURE_LEVEL			m_FeatureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11DeviceContext*		m_pImmediateContext = nullptr;
	ID3D11RenderTargetView*		m_pRenderTargetView = nullptr;
	ID3D11Texture2D*			m_pDepthStencil = nullptr;
	ID3D11DepthStencilView*		m_pDepthStencilView = nullptr;

	ID3D11VertexShader*         m_pVertexShader = nullptr;
	ID3D11InputLayout*          m_pVertexInputLayout = nullptr;
	ID3D11PixelShader*          m_pPixelShader = nullptr;

};

