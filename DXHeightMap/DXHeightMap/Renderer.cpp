#include "pch.h"
#include "Renderer.h"
#include "WndInfo.h"
#include "Application.h"


Renderer::Renderer()
{
	Init();
}


Renderer::~Renderer()
{
	ReleaseCOM(m_pRenderTargetView);
	ReleaseCOM(m_pDepthStencilView);
	ReleaseCOM(m_pSwapChain);
	ReleaseCOM(m_pDepthStencilBuffer);

	if (m_pImmediateContext)
		m_pImmediateContext->ClearState();
	ReleaseCOM(m_pImmediateContext);
	ReleaseCOM(m_pDevice);
}

bool Renderer::Init()
{
	HRESULT hr = S_OK;

	// device �ʱ�ȭ 
	UINT     createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif	
	D3D_FEATURE_LEVEL     featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT     numFeatureLevels = ARRAYSIZE(featureLevels);


	/// swap chain �κ�
	//DXGI_SWAP_CHAIN_DESC sd;
	//ZeroMemory(&sd, sizeof(sd));
	//sd.BufferCount = 1;

	//// ũ��� ��������� �������� 
	//sd.BufferDesc.Width = WNDWIDTH;
	//sd.BufferDesc.Height = WNDHEIGHT;
	//sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// ����� ���� 
	//sd.BufferDesc.RefreshRate.Numerator = 60; 	 // ����
	//sd.BufferDesc.RefreshRate.Denominator = 1; 	// �и�
	//sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		 // ����� ������

	//// director�� ��� �;��� 
	////sd.OutputWindow = g_hWnd;		// ���� ������
	//sd.SampleDesc.Count = 1;
	//sd.SampleDesc.Quality = 0;
	//sd.Windowed = TRUE;

	 hr = D3D11CreateDevice(
		nullptr,
		m_D3DDriverType,
		0,
		createDeviceFlags,
		featureLevels,
		numFeatureLevels,
		D3D11_SDK_VERSION,
		&m_pDevice,
		&m_FeatureLevel,
		&m_pImmediateContext);

	if (FAILED(hr))
	{
		MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
		return false;
	}

	if (m_FeatureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
		return false;
	}

	HR(m_pDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_4xMsaaQuality));
	_ASSERT(m_4xMsaaQuality > 0);

	DXGI_SWAP_CHAIN_DESC sd;

	sd.BufferDesc.Width = WNDWIDTH;
	sd.BufferDesc.Height = WNDHEIGHT;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	if (m_Enable4XMsaa)
	{
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = m_4xMsaaQuality - 1;
	}
	else
	{
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = Application::GetInstance()->GetWindowHandle();
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;


	IDXGIDevice* dxgiDevice = 0;
	HR(m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));

	IDXGIAdapter* dxgiAdapter = 0;
	HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));

	IDXGIFactory* dxgiFactory = 0;
	HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));

	HR(dxgiFactory->CreateSwapChain(m_pDevice, &sd, &m_pSwapChain));

	ReleaseCOM(dxgiFactory);
	ReleaseCOM(dxgiAdapter);
	ReleaseCOM(dxgiDevice);

	//hr = D3D11CreateDeviceAndSwapChain(
	//	0,  		 // �⺻ ���÷��� ����� ���
	//	D3D_DRIVER_TYPE_HARDWARE, 	// 3D �ϵ���� ����
	//	0,  		// ����Ʈ���� ���� ����
	//	createDeviceFlags,
	//	featureLevels,
	//	numFeatureLevels,
	//	D3D11_SDK_VERSION,  // SDK version
	//	&sd, // Swap chain description
	//	&m_pSwapChain,
	//	&m_pDevice,
	//	&m_FeatureLevel,
	//	&m_pImmediateContext);

	if (FAILED(hr))
		return false;
	//////////

	/// Create a render target view
	ID3D11Texture2D*      pBackBuffer = nullptr;
	hr = m_pSwapChain->GetBuffer(0,			// �ĸ� ���� ����. �������϶� �߿�. ������ 1���̹Ƿ� 0.
								 __uuidof(ID3D11Texture2D),  	// ���� ����
								(LPVOID*)&pBackBuffer);         // �޾ƿ� ����
	
	if (FAILED(hr))
		return false;

	hr = m_pDevice->CreateRenderTargetView(pBackBuffer,
		NULL, 		// �ڿ� ���� 
		&m_pRenderTargetView);
	if (FAILED(hr))
		return false;

	pBackBuffer->Release();		// Get�� �ؿ����� �ݵ�� Release
	///////

	/// Output Merge / setup viewport
	m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);

	D3D11_VIEWPORT vp;
	vp.Width = WNDWIDTH;      // ����Ʈ �ʺ�
	vp.Height = WNDHEIGHT;   // ����Ʈ ����
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0; 		// �׸��� ���� ���� x
	vp.TopLeftY = 0; 		// �׸��� ���� ���� y
	m_pImmediateContext->RSSetViewports(1, &vp);

	// clearColor ����
	for (int i = 0; i < 4; ++i)
	{
		m_ClearColor[i] = 0.1f;
	}

	return true;

}

void Renderer::Render()
{

	// Clear
	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, m_ClearColor);

	// Render
	m_pSwapChain->Present(0, 0);           // ù��° ���� : ���� ������
}

