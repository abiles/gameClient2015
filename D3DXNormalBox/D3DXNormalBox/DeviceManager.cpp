#include "pch.h"
#include "DeviceManager.h"
#include "WindowManager.h"


DeviceManager::DeviceManager()
{
}


DeviceManager::~DeviceManager()
{
}

HRESULT DeviceManager::initDevice()
{
	HRESULT hr = S_OK;

	hr = createSwapChain();

	if (FAILED(hr))
		return hr;

	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&m_pBackBuffer);

	if (FAILED(hr))
		return hr;

	hr = m_pD3dDevice->CreateRenderTargetView(m_pBackBuffer, NULL, &m_pRenderTargetView);

	m_pBackBuffer->Release();

	if (FAILED(hr))
		return hr;

	hr = createDepthStencilTexture();
	
	if (FAILED(hr))
		return hr;

	// 첫번째 인자는 분할된 화면의 개수를 말한다
	// 세번째 인자는 깊이와 스탠실 버퍼에 대한 내용

	m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	setViewport();
	
	if (FAILED(hr))
		return hr;
	
	hr = createShader();
	
	if (FAILED(hr))
		return hr;

	hr = createRenderState();

	if (FAILED(hr))
		return hr;

	return hr;
}

HRESULT DeviceManager::createDepthStencilTexture()
{
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = 800;
	descDepth.Height = 600;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	HRESULT hr = m_pD3dDevice->CreateTexture2D(&descDepth, NULL, &m_pDepthStencil);
	if (FAILED(hr))
		return hr;

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC 	descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	descDSV.Flags = 0;
	hr = m_pD3dDevice->CreateDepthStencilView(m_pDepthStencil, &descDSV, &m_pDepthStencilView);

	if (FAILED(hr))
		return hr;

	return hr;

}

HRESULT DeviceManager::createShader()
{
	ID3DBlob* pVSBlob = nullptr;
	ID3DBlob* pErrorBlob = nullptr;

	HRESULT hr = D3DX11CompileFromFile(
		L"./MyShader.fx" // shader 파일 이름
		, 0, 0
		, "VS"     // 그중 컴파일하려는 함수
		, "vs_5_0" // 컴파일 버전
		, 0, 0, 0
		, &pVSBlob
		, &pErrorBlob
		, 0);

	hr = m_pD3dDevice->CreateVertexShader(
		pVSBlob->GetBufferPointer()
		, pVSBlob->GetBufferSize()
		, 0
		, &m_pVertexShader);

	if (FAILED(hr))
		return hr;

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	UINT numElements = ARRAYSIZE(layout);

	//이렇게 만들어진 layout을 VS 설정에 연결해주면 된다
	hr = m_pD3dDevice->CreateInputLayout(
		layout
		, numElements
		, pVSBlob->GetBufferPointer()
		, pVSBlob->GetBufferSize()
		, &m_pVertexInputLayout);

	pVSBlob->Release();


	ID3DBlob* pPSBlob = nullptr;

	hr = D3DX11CompileFromFile(
		L"MyShader.fx"
		, 0, 0
		, "PS"
		, "ps_5_0"
		, 0, 0, 0
		, &pPSBlob
		, &pErrorBlob
		, 0);
	
	if (FAILED(hr))
		return hr;

	hr = m_pD3dDevice->CreatePixelShader(
		pPSBlob->GetBufferPointer()
		, pPSBlob->GetBufferSize()
		, 0
		, &m_pPixelShader);

	pPSBlob->Release();

	if (FAILED(hr))
		return hr;

	return hr;

}

void DeviceManager::setViewport()
{
	D3D11_VIEWPORT vp;
	vp.Width = WindowManager::getInstance()->getWndSize().m_MaxWidth;
	vp.Height = WindowManager::getInstance()->getWndSize().m_MaxHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0; // renderTaget 그리기 시작 원점
	vp.TopLeftY = 0;
	m_pImmediateContext->RSSetViewports(1, &vp);
}

HRESULT DeviceManager::createSwapChain()
{
	HRESULT hr = S_OK;

	// flag 설정
	UINT createDeviceFlags = 0;

#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// 설정 레벨 dx11이냐 10이냐 이런거
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1; // back buffer 몇개 쓸거냐
	sd.BufferDesc.Width = WindowManager::getInstance()->getWndSize().m_MaxWidth;
	sd.BufferDesc.Height = WindowManager::getInstance()->getWndSize().m_MaxHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = WindowManager::getInstance()->getHWND();
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	// 동시에 Device와 SwapChain을 만든다
	hr = D3D11CreateDeviceAndSwapChain(
		0,
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		createDeviceFlags,
		featureLevels,
		numFeatureLevels,
		D3D11_SDK_VERSION,
		&sd,
		&m_pSwapChain,
		&m_pD3dDevice,
		&m_FeatureLevel,
		&m_pImmediateContext);

	if (FAILED(hr))
		return hr;
	
	return hr;
}

DeviceManager* DeviceManager::getInstance()
{
	static DeviceManager deviceManager;

	return &deviceManager;
}

ID3D11Device* DeviceManager::getD3D11Device() const
{
	return m_pD3dDevice;
}

HRESULT DeviceManager::createRenderState()
{
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_FRONT;
	rasterizerDesc.FrontCounterClockwise = false;
	HRESULT hr = m_pD3dDevice->CreateRasterizerState(&rasterizerDesc, &m_pSolidRS);

	return hr;
}

ID3D11DeviceContext* DeviceManager::getD3D11DeviceContext() const
{
	return m_pImmediateContext;
}

void DeviceManager::render(float deltaTime, ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer)
{
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };

	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor);

	m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// Render
	// Present 할 때마다 back buffer와 포인터를 swap한다

	// set input Assembler
	m_pImmediateContext->IASetInputLayout(m_pVertexInputLayout);  // 어떤 레이아웃으로 쓸꺼냐

	//점을 선택하는 방식을 어떻게 할꺼냐
	m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT stride = sizeof(MyVertex);
	UINT offset = 0;
	m_pImmediateContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	m_pImmediateContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//set shader and draw
	m_pImmediateContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pImmediateContext->PSSetShader(m_pPixelShader, NULL, 0);

	m_pImmediateContext->DrawIndexed(36, 0, 0);

	m_pSwapChain->Present(0, 0); // 첫번째 인자 : 갱신 딜레이, 0이 제일 빠르다 
}
