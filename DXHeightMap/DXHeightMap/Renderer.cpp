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


	// device 초기화 
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


	/// swap chain 부분
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;

	// 크기는 상수값으로 빼놔야지 
	sd.BufferDesc.Width = 800;
	sd.BufferDesc.Height = 600;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// 백버퍼 포맷 
	sd.BufferDesc.RefreshRate.Numerator = 60; 	 // 분자
	sd.BufferDesc.RefreshRate.Denominator = 1; 	// 분모
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		 // 백버퍼 렌더링

	// director로 얻어 와야지 
	//sd.OutputWindow = g_hWnd;		// 현재 윈도우
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;


	hr = D3D11CreateDeviceAndSwapChain(
		0,  		 // 기본 디스플레이 어댑터 사용
		D3D_DRIVER_TYPE_HARDWARE, 	// 3D 하드웨어 가속
		0,  		// 소프트웨어 구동 안함
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
	hr = m_pSwapChain->GetBuffer(0,			// 후면 버퍼 색인. 여러개일때 중요. 지금은 1개이므로 0.
								 __uuidof(ID3D11Texture2D),  	// 버퍼 형식
								(LPVOID*)&pBackBuffer);         // 받아온 버퍼
	
	if (FAILED(hr))
		return false;

	hr = m_pDevice->CreateRenderTargetView(pBackBuffer,
		NULL, 		// 자원 형식 
		&m_pRenderTargetView);
	if (FAILED(hr))
		return false;

	pBackBuffer->Release();		// Get을 해왔으면 반드시 Release
	///////

	/// Output Merge / setup viewport
	m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);

	D3D11_VIEWPORT vp;
	vp.Width = 800;      // 뷰포트 너비
	vp.Height = 600;   // 뷰포트 높이
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0; 		// 그리기 시작 원점 x
	vp.TopLeftY = 0; 		// 그리기 시작 원점 y
	m_pImmediateContext->RSSetViewports(1, &vp);

}

