#include "pch.h"
#include "Renderer.h"


Renderer::Renderer()
{
}


Renderer::~Renderer()
{
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
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;

	// ũ��� ��������� �������� 
	sd.BufferDesc.Width = 800;
	sd.BufferDesc.Height = 600;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// ����� ���� 
	sd.BufferDesc.RefreshRate.Numerator = 60; 	 // ����
	sd.BufferDesc.RefreshRate.Denominator = 1; 	// �и�
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		 // ����� ������

	// director�� ��� �;��� 
	//sd.OutputWindow = g_hWnd;		// ���� ������
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;


	hr = D3D11CreateDeviceAndSwapChain(
		0,  		 // �⺻ ���÷��� ����� ���
		D3D_DRIVER_TYPE_HARDWARE, 	// 3D �ϵ���� ����
		0,  		// ����Ʈ���� ���� ����
		createDeviceFlags,
		featureLevels,
		numFeatureLevels,
		D3D11_SDK_VERSION,  // SDK version
		&sd, // Swap chain description
		&m_pSwapChain,
		&m_pDevice,
		&m_FeatureLevel,
		&m_pImmediateContext);

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
	vp.Width = 800;      // ����Ʈ �ʺ�
	vp.Height = 600;   // ����Ʈ ����
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0; 		// �׸��� ���� ���� x
	vp.TopLeftY = 0; 		// �׸��� ���� ���� y
	m_pImmediateContext->RSSetViewports(1, &vp);

}

