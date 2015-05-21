#include "pch.h"
#include "HeightMap.h"
#include "Renderer.h"
#include "HeightMapShader.h"
#include "Director.h"
#include "Camera.h"

HeightMap::HeightMap()
{
}


HeightMap::~HeightMap()
{
}

void HeightMap::Init()
{
	Node::InitMatrix();

	LoadHeightMap();
	CreateVertexLayOut();
	CreateVertexBuffer();
	CreateIndexBuffer();
	CreateConstantBuffer();
	LoadTexture();
	CreateRenderState();
}

void HeightMap::LoadHeightMap()
{
	std::vector<BYTE>  DataFromFile(m_NumOfVertecies);
	std::ifstream loadFile;
	loadFile.open(L"Texture/heightMap.raw", std::ios_base::binary);

	if (loadFile)
	{
		loadFile.read((char*)&DataFromFile[0], (std::streamsize)DataFromFile.size());

		loadFile.close();
	}

	m_HeightMap.resize(m_NumOfVertecies);

	for (int i = 0; i < DataFromFile.size(); ++i)
	{
		m_HeightMap[i] = DataFromFile[i];
	}

}

void HeightMap::CreateVertexLayOut()
{
	D3D11_INPUT_ELEMENT_DESC	 layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	UINT   numElements = ARRAYSIZE(layout);
	ID3DBlob* vsBlob = GET_HM_SHADER()->GetVSBlob();

	HRESULT hr = GET_DEVICE()->CreateInputLayout(layout, numElements, vsBlob->GetBufferPointer(),
	vsBlob->GetBufferSize(), &m_InputLayout);

	if (hr != S_OK)
	{
		char buffer[1000] = { 0, };
		sprintf_s(buffer, 1000, " last error : %d", GetLastError());
		MessageBoxA(NULL, buffer, "hr error", MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2);
	}
	
}

void HeightMap::CreateVertexBuffer()
{
	HeightVertex* heightmapVertex = new HeightVertex[m_NumOfVertecies];

	for (int z = 0; z < m_VertexCount; ++z)
	{
		for (int x = 0; x < m_VertexCount; ++x)
		{
			int   idx = x + (z * (m_VertexCount));
			heightmapVertex[idx].m_Pos = XMFLOAT3(x, m_HeightMap[idx], z);
			
			// 이부분에 색깔이랑 tex안넣어줘도 되는지.?
			heightmapVertex[idx].m_Color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			heightmapVertex[idx].m_Tex = XMFLOAT2(x / (float)(m_VertexCount - 1), z / (float)(m_VertexCount - 1));

		}
	}

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(HeightVertex)* m_NumOfVertecies;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = heightmapVertex;
	HRESULT hr = GET_DEVICE()->CreateBuffer(&bd, &InitData, &m_VertexBuffer);
	
	if (hr != S_OK)
	{
		char buffer[1000] = { 0, };
		sprintf_s(buffer, 1000, "last error : %d", GetLastError());
		MessageBoxA(NULL, buffer, "hr error", MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2);
	}

}


void HeightMap::CreateIndexBuffer()
{
	int triangleCount = (m_VertexCount - 1) * (m_VertexCount - 1) * 2;    // 삼각형 개수
	m_IndexSize = triangleCount * 3;
	UINT* indices = new UINT[m_IndexSize];

	int baseIndex = 0;
	int _numVertsPerRow = m_VertexCount;

	for (int z = 0; z < _numVertsPerRow - 1; z++)
	{
		for (int x = 0; x < _numVertsPerRow - 1; x++)
		{
			indices[baseIndex] = z   * _numVertsPerRow + x;            // 0
			indices[baseIndex + 2] = z   * _numVertsPerRow + x + 1;    //  3
			indices[baseIndex + 1] = (z + 1) * _numVertsPerRow + x;   //  1

			indices[baseIndex + 3] = (z + 1) * _numVertsPerRow + x;   // 3
			indices[baseIndex + 5] = z   * _numVertsPerRow + x + 1;    //  4
			indices[baseIndex + 4] = (z + 1) * _numVertsPerRow + x + 1;   // 1

			baseIndex += 6;
		}
	}

	D3D11_BUFFER_DESC ibd;
	ZeroMemory(&ibd, sizeof(ibd));
	ibd.ByteWidth = sizeof(UINT)* m_IndexSize;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;
	ZeroMemory(&iinitData, sizeof(iinitData));
	iinitData.pSysMem = indices;
	HRESULT hr = GET_DEVICE()->CreateBuffer(&ibd, &iinitData, &m_IndexBuffer);

	if (hr != S_OK)
	{
		char buffer[1000] = { 0, };
		sprintf_s(buffer, 1000, "last error : %d", GetLastError());
		MessageBoxA(NULL, buffer, "hr error", MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2);
	}


}


void HeightMap::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC 	cbd;
	ZeroMemory(&cbd, sizeof(cbd));
	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.ByteWidth = sizeof(ConstantBuffer);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = 0;

	HRESULT hr = GET_DEVICE()->CreateBuffer(&cbd, NULL, &m_ConstantBuffer);
	if (hr != S_OK)
	{
		char buffer[1000] = { 0, };
		sprintf_s(buffer, 1000, "last error : %d", GetLastError());
		MessageBoxA(NULL, buffer, "hr error", MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2);
	}

}

void HeightMap::LoadTexture()
{
	HRESULT   hr = D3DX11CreateShaderResourceViewFromFile(
		GET_DEVICE(),
		L"Texture/heightMap.jpg", 			
		NULL, 			
		NULL,					
		&m_TextureRV, 	
		NULL);
	
	if (hr != S_OK)
	{
		char buffer[1000] = { 0, };
		sprintf_s(buffer, 1000, "last error : %d", GetLastError());
		MessageBoxA(NULL, buffer, "hr error", MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2);
	}

	D3D11_SAMPLER_DESC 	sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = GET_DEVICE()->CreateSamplerState(&sampDesc, &m_SamplerLinear); // SamplerState 생성

	if (hr != S_OK)
	{
		char buffer[1000] = { 0, };
		sprintf_s(buffer, 1000, "last error : %d", GetLastError());
		MessageBoxA(NULL, buffer, "hr error", MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2);
	}
}

void HeightMap::DrawByVertex()
{


}

void HeightMap::DrawByIndex()
{
	XMMATRIX world = XMLoadFloat4x4(&m_World);
	XMMATRIX wvp = world * GET_DIRECTOR()->GetCamera()->ViewProj();
	
	ConstantBuffer cb;
	XMStoreFloat4x4(&cb.m_WVP, XMMatrixTranspose(wvp));
	XMStoreFloat4x4(&cb.m_World, XMMatrixTranspose(world));

	GET_DEVICECONTEXT()->UpdateSubresource(m_ConstantBuffer, 0, 0, &cb, 0, 0);
	GET_DEVICECONTEXT()->VSSetConstantBuffers(0, 1, &m_ConstantBuffer);

	// Set Input Assembler 
	GET_DEVICECONTEXT()->IASetInputLayout(m_InputLayout);
	GET_DEVICECONTEXT()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	// Set Shader and Draw
	GET_DEVICECONTEXT()->VSSetShader(GET_HM_SHADER()->GetVertexShader(), NULL, 0);
	GET_DEVICECONTEXT()->PSSetShader(GET_HM_SHADER()->GetPixelShader(), NULL, 0);
	GET_DEVICECONTEXT()->PSSetShaderResources(0, 1, &m_TextureRV);
	GET_DEVICECONTEXT()->PSSetSamplers(0, 1, &m_SamplerLinear);

	GET_DEVICECONTEXT()->RSSetState(m_SolidRS);

	UINT stride = sizeof(HeightVertex);
	UINT offset = 0;
	GET_DEVICECONTEXT()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
	GET_DEVICECONTEXT()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	GET_DEVICECONTEXT()->DrawIndexed(m_IndexSize, 0, 0);
}

void HeightMap::CreateRenderState()
{
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;		// Fill 옵션
	rasterizerDesc.CullMode = D3D11_CULL_FRONT;	// Culling 옵션
	rasterizerDesc.FrontCounterClockwise = false;		// 앞/뒷면 로직 선택

	GET_DEVICE()->CreateRasterizerState(&rasterizerDesc, &m_SolidRS);
}
