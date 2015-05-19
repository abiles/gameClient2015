#include "pch.h"
#include "Renderer.h"
#include "Application.h"
#include "Shader.h"
#include "Director.h"
#include "Node.h"

Renderer::Renderer()
{
	Init();
}


Renderer::~Renderer()
{
	ReleaseCOM(m_D3dDevice);
	ReleaseCOM(m_D3dImmediateContext);
	ReleaseCOM(m_SwapChain);
	ReleaseCOM(m_DepthStencilView);
	ReleaseCOM(m_DepthStencilBuffer);
	ReleaseCOM(m_RenderTargetView);
}

bool Renderer::Init()
{
	for (int i = 0; i < 4; i++)
	{
		m_ClearColor[i] = 0.5f;
	}

	ZeroMemory(&m_ScreenViewPort, sizeof(D3D11_VIEWPORT));

	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};

	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	D3D_FEATURE_LEVEL featureLevel;


	HRESULT hr = D3D11CreateDevice(
		nullptr,                 // default adapter
		m_D3dDriverType,
		0,                 // no software device
		createDeviceFlags,
		featureLevels, numFeatureLevels,              // default feature level array
		D3D11_SDK_VERSION,
		&m_D3dDevice,
		&featureLevel,
		&m_D3dImmediateContext);

	//assert(GetLastError() == 0);

	if (FAILED(hr))
	{
		MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
		return false;
	}

	if (featureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
		return false;
	}

	// Check 4X MSAA quality support for our back buffer format.
	// All Direct3D 11 capable devices support 4X MSAA for all render 
	// target formats, so we only need to check quality support.

	HR(m_D3dDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_4xMsaaQuality));
	//assert(m_4xMsaaQuality > 0);

	// Fill out a DXGI_SWAP_CHAIN_DESC to describe our swap chain.

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = Application::GetInstance()->GetWndWidth();
	sd.BufferDesc.Height = Application::GetInstance()->GetWndHeight();
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.OutputWindow = Application::GetInstance()->GetWndHandle();
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Use 4X MSAA? 
	if (m_Enable4XMsaa)
	{
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = m_4xMsaaQuality - 1;
	}
	// No MSAA
	else
	{
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = Application::GetInstance()->GetWndHandle();
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	// To correctly create the swap chain, we must use the IDXGIFactory that was
	// used to create the device.  If we tried to use a different IDXGIFactory instance
	// (by calling CreateDXGIFactory), we get an error: "IDXGIFactory::CreateSwapChain: 
	// This function is being called with a device from a different IDXGIFactory."

	IDXGIDevice* dxgiDevice = 0;
	HR(m_D3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));
	int r = GetLastError();
	assert(GetLastError() == 0);

	IDXGIAdapter* dxgiAdapter = 0;
	HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));
	int r2 = GetLastError();
	assert(GetLastError() == 0);

	IDXGIFactory* dxgiFactory = 0;
	HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));
	int r3 = GetLastError();
	assert(GetLastError() == 0);

	HR(dxgiFactory->CreateSwapChain(m_D3dDevice, &sd, &m_SwapChain));

	int r4 = GetLastError();
	//assert(GetLastError() == 0);
	ReleaseCOM(dxgiDevice);
	ReleaseCOM(dxgiAdapter);
	ReleaseCOM(dxgiFactory);

	// The remaining steps that need to be carried out for d3d creation
	// also need to be executed every time the window is resized.  So
	// just call the OnResize method here to avoid code duplication


	OnResize();
	
	return true;
}

void Renderer::Render(Node& node)
{
	float        ClearColor[4] = { 0.0f, 0.0f, 0.5f, 1.0f }; //red,green,blue,alpha

	// Clear
	m_D3dImmediateContext->ClearRenderTargetView(m_RenderTargetView, ClearColor);

	node.DrawByIndex();

	// Render
	m_SwapChain->Present(0, 0);           // 첫번째 인자 : 갱신 딜레이 
}

void Renderer::OnResize()
{
	assert(m_D3dImmediateContext);
	assert(m_D3dDevice);
	assert(m_SwapChain);

	ReleaseCOM(m_RenderTargetView);
	ReleaseCOM(m_DepthStencilView);
	ReleaseCOM(m_DepthStencilBuffer);

	HR(m_SwapChain->ResizeBuffers(1, 
								  Application::GetInstance()->GetWndWidth(),
								  Application::GetInstance()->GetWndHeight(),
								  DXGI_FORMAT_R8G8B8A8_UNORM, 0));

	ID3D11Texture2D*  backBuffer = nullptr;
	HR(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	HR(m_D3dDevice->CreateRenderTargetView(backBuffer, 0, &m_RenderTargetView));
	ReleaseCOM(backBuffer);

	 //depth stencil buffer, view  만들기
	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = Application::GetInstance()->GetWndWidth();
	depthStencilDesc.Height = Application::GetInstance()->GetWndHeight();
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	if (m_Enable4XMsaa)
	{
		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = m_4xMsaaQuality - 1;
	}
	// No MSAA
	else
	{
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	HR(m_D3dDevice->CreateTexture2D(&depthStencilDesc, 0, &m_DepthStencilBuffer));
	HR(m_D3dDevice->CreateDepthStencilView(m_DepthStencilBuffer, 0, &m_DepthStencilView));


	 //Bind the render target view and depth/stencil view to the pipeline.

	m_D3dImmediateContext->OMSetRenderTargets(1, &m_RenderTargetView, NULL);


	// Set the viewport transform.

	m_ScreenViewPort.TopLeftX = 0;
	m_ScreenViewPort.TopLeftY = 0;
	m_ScreenViewPort.Width = static_cast<float>(Application::GetInstance()->GetWndWidth());
	m_ScreenViewPort.Height = static_cast<float>(Application::GetInstance()->GetWndHeight());
	m_ScreenViewPort.MinDepth = 0.0f;
	m_ScreenViewPort.MaxDepth = 1.0f;

	m_D3dImmediateContext->RSSetViewports(1, &m_ScreenViewPort);
}

ID3D11Device* Renderer::GetDevice() const
{
	return m_D3dDevice;
}

Renderer* Renderer::GetInstance()
{
	static Renderer SingletonRenderer;

	return &SingletonRenderer;
}

ID3D11DeviceContext* Renderer::GetDeviceContext() const
{
	return m_D3dImmediateContext;
}
