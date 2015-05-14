#include "pch.h"
#include "BoxApp.h"

BoxApp::BoxApp(HINSTANCE hInstance)
: D3DXApp(hInstance)
{

	m_MainWndTitle = L"Box Demo";

	m_LastMousePoint.x = 0;
	m_LastMousePoint.y = 0;

	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&m_World, I);
	XMStoreFloat4x4(&m_View, I);
	XMStoreFloat4x4(&m_Proj, I);

}


BoxApp::~BoxApp()
{
	ReleaseCOM(m_BoxVB);
	ReleaseCOM(m_BoxIB);
	ReleaseCOM(m_FX);
	ReleaseCOM(m_InputLayout);
}

bool BoxApp::init()
{
	if (!D3DXApp::init())
		return false;

	buildGeometryBuffers();
	buildFX();
	buildVertexLayout();

	return true;
}

void BoxApp::onResize()
{
	D3DXApp::onResize();


	//???
	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * MathHelper::Pi,
										  aspectRatio(), 1.0f, 1000.f);
	XMStoreFloat4x4(&m_Proj, P);

}

void BoxApp::updateScene(float dt)
{
	float x = m_Radiuos * sinf(m_Phi) * cosf(m_Theta);
	float z = m_Radiuos * sinf(m_Phi) * sinf(m_Theta);
	float y = m_Radiuos * cosf(m_Theta);

	//시야 행렬을 구축한다.
	XMVECTOR pos = XMVectorSet(x, y, z, 1.0f);
	XMVECTOR target = XMVectorZero();
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX V = XMMatrixLookAtLH(pos, target, up);
	XMStoreFloat4x4(&m_View, V);

}

void BoxApp::drawScene()
{
	m_D3dImmediateContext->ClearRenderTargetView(m_RenderTargetView,
		reinterpret_cast<const float*>(&Colors::Blue));

	m_D3dImmediateContext->ClearDepthStencilView(m_DepthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	m_D3dImmediateContext->IASetInputLayout(m_InputLayout);
	m_D3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	m_D3dImmediateContext->IASetVertexBuffers(0, 1, &m_BoxVB, &stride, &offset);
	m_D3dImmediateContext->IASetIndexBuffer(m_BoxIB, DXGI_FORMAT_R32_UINT, 0);

	XMMATRIX world = XMLoadFloat4x4(&m_World);
	XMMATRIX view = XMLoadFloat4x4(&m_View);
	XMMATRIX proj = XMLoadFloat4x4(&m_Proj);
	XMMATRIX worldViewProj = world*view*proj;

	m_FXWorldViewProj->SetMatrix(reinterpret_cast<float*>(&worldViewProj));

	D3DX11_TECHNIQUE_DESC techDesc;
	m_Tech->GetDesc(&techDesc);

	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_Tech->GetPassByIndex(p)->Apply(0, m_D3dImmediateContext);

		// 색인 36개로 상자그리기
		m_D3dImmediateContext->DrawIndexed(36, 0, 0);
	}

	HR(m_SwapChain->Present(0, 0));

}

void BoxApp::onMouseDown(WPARAM btnState, int x, int y)
{
	m_LastMousePoint.x = x;
	m_LastMousePoint.y = y;

	SetCapture(m_hMainWnd);
}

void BoxApp::onMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void BoxApp::onMouseMove(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		float dx = XMConvertToRadians(0.25f * static_cast<float>(x - m_LastMousePoint.x));
		float dy = XMConvertToRadians(0.25f * static_cast<float>(y - m_LastMousePoint.y));

		m_Theta += dx;
		m_Phi += dy;

		m_Phi = MathHelper::Clamp(m_Phi, 0.1f, MathHelper::Pi - 0.1f);
	}
	else if ((btnState & MK_RBUTTON) != 0)
	{
		float dx = 0.005f*static_cast<float>(x - m_LastMousePoint.x);
		float dy = 0.005f*static_cast<float>(y - m_LastMousePoint.y);

		m_Radiuos += dx - dy;

		m_Radiuos = MathHelper::Clamp(m_Radiuos, 3.0f, 15.0f);

	}

	m_LastMousePoint.x = x;
	m_LastMousePoint.y = y;
}

void BoxApp::buildGeometryBuffers()
{
	//정점 버퍼 생성

	Vertex vertices[] =
	{
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), (const float*)&Colors::White },
		{ XMFLOAT3(-1.0f, +1.0f, -1.0f), (const float*)&Colors::Black },
		{ XMFLOAT3(+1.0f, +1.0f, -1.0f), (const float*)&Colors::Blue },
		{ XMFLOAT3(+1.0f, -1.0f, -1.0f), (const float*)&Colors::Red },
		{ XMFLOAT3(-1.0f, -1.0f, +1.0f), (const float*)&Colors::Green },
		{ XMFLOAT3(-1.0f, +1.0f, +1.0f), (const float*)&Colors::Yellow },
		{ XMFLOAT3(+1.0f, +1.0f, +1.0f), (const float*)&Colors::Cyan },
		{ XMFLOAT3(+1.0f, -1.0f, +1.0f), (const float*)&Colors::Magenta },
	};

	D3D11_BUFFER_DESC vbd = {};
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex)* 8;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitdata;
	vinitdata.pSysMem = vertices;
	HR(m_D3dDevice->CreateBuffer(&vbd, &vinitdata, &m_BoxVB));

	//색인과 버퍼 생성
	UINT indices[] =
	{
		// 앞면
		0, 1, 2,
		0, 2, 3,

		// 뒷
		4, 6, 5,
		4, 7, 6,

		//왼쪽
		4, 5, 1,
		4, 1, 0,

		//오른쪽
		3, 2, 6,
		3, 6, 7,

		//윗면
		1, 5, 6,
		1, 6, 2,


		//밑면
		4, 0, 3,
		4, 3, 7
	};

	D3D11_BUFFER_DESC ibd = {};
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT)* 36;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iInitdata;
	iInitdata.pSysMem = indices;
	HR(m_D3dDevice->CreateBuffer(&ibd, &iInitdata, &m_BoxIB));

}

void BoxApp::buildFX()
{
	DWORD shaderFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	ID3D10Blob* compiledShader = nullptr;
	ID3D10Blob* compilationMsgs = nullptr;

	HRESULT hr = D3DX11CompileFromFile(L"FX/color.fx", 0, 0, 0,
		"fx_5_0", shaderFlags, 0, 0, &compiledShader, &compilationMsgs, 0);

	if (compilationMsgs != 0)
	{
		MessageBox(0, (LPCWSTR)compilationMsgs->GetBufferPointer(), 0, 0);
		ReleaseCOM(compilationMsgs);
	}

	if (FAILED(hr))
	{
		DXTrace(__FILE__, (DWORD)__LINE__, hr, L"D3DX11CompileFromFile", true);
	}

	HR(D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(),
		compiledShader->GetBufferSize(),
		0, m_D3dDevice, &m_FX));

	ReleaseCOM(compiledShader);

	m_Tech = m_FX->GetTechniqueByName("ColorTech");
	m_FXWorldViewProj = m_FX->GetVariableByName("gWorldViewProj")->AsMatrix();

}

void BoxApp::buildVertexLayout()
{
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] = 
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	D3DX11_PASS_DESC passDesc;
	m_Tech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(m_D3dDevice->CreateInputLayout(vertexDesc, 2, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &m_InputLayout));
}

