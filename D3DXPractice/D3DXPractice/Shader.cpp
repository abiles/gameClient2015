#include "pch.h"
#include "Shader.h"
#include "Director.h"
#include "Renderer.h"


Shader::Shader()
{
	CreateShader();
	CreateVertexBuffer();
	CreateIndexBuffer();
}


Shader::~Shader()
{
	ReleaseCOM(m_VertexBuffer);
	ReleaseCOM(m_IndexBuffer);
	ReleaseCOM(m_VertexLayout);
	ReleaseCOM(m_VertexShader);
	ReleaseCOM(m_PixelShader);
	ReleaseCOM(m_VSBlob);
	ReleaseCOM(m_ErrorBlob);

}

void Shader::CreateShader()
{
	CreateVertexShader();
	CreatePixelShader();
}

void Shader::CreateVertexShader()
{

	HRESULT hr = D3DX11CompileFromFile(L"MyShader.fx", 0, 0,
		"VS", "vs_5_0", 0,
		0, 0,
		&m_VSBlob, &m_ErrorBlob, 0);

	hr = GET_DEVICE()->CreateVertexShader(
		m_VSBlob->GetBufferPointer(),
		m_VSBlob->GetBufferSize(),
		nullptr,
		&m_VertexShader);


	//////////// layout
	D3D11_INPUT_ELEMENT_DESC	 layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },

	};
	//////////////////

	UINT   numElements = ARRAYSIZE(layout);

	hr = GET_DEVICE()->CreateInputLayout(layout, numElements, m_VSBlob->GetBufferPointer(),
		m_VSBlob->GetBufferSize(), &m_VertexLayout);

}

void Shader::CreatePixelShader()
{
	// PixelShader
	ID3DBlob* PSBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	D3DX11CompileFromFile(L"MyShader.fx", 0, 0,
		"PS", "ps_5_0", 0,
		0, 0,
		&PSBlob, &errorBlob, 0);

	GET_DEVICE()->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(),
		0, &m_PixelShader);

	PSBlob->Release();
}

void Shader::CreateVertexBuffer()
{
	MyVertex	 vertices[] =
	{
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },


	};


	D3D11_BUFFER_DESC    bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(vertices); 	
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;


	D3D11_SUBRESOURCE_DATA    InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;
	GET_DEVICE()->CreateBuffer(&bd, &InitData, &m_VertexBuffer);

}

ID3D11VertexShader* Shader::GetVertexShader()
{
	return m_VertexShader;
}

ID3D11InputLayout* Shader::GetVertexLayout()
{
	return m_VertexLayout;
}

ID3D11PixelShader* Shader::GetPixelShader()
{
	return m_PixelShader;
}

ID3D11Buffer* Shader::GetVertexBuffer()
{
	return m_VertexBuffer;
}

void Shader::CreateIndexBuffer()
{
	UINT indices[] =
	{
		3, 1, 0,
		2, 1, 3,
		0, 5, 4,
		1, 5, 0,
		3, 4, 7,
		0, 4, 3,
		1, 6, 5,
		2, 6, 1,
		2, 7, 6,
		3, 7, 2,
		6, 4, 5,
		7, 4, 6,
	};


	D3D11_BUFFER_DESC 	ibd;
	ZeroMemory(&ibd, sizeof(ibd));
	ibd.ByteWidth = sizeof(indices);
	ibd.Usage = D3D11_USAGE_IMMUTABLE;	// CPU 접근 불가. 생성후 변경 불가. GPU만 접근 가능
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA 		iinitData;
	ZeroMemory(&iinitData, sizeof(iinitData));
	iinitData.pSysMem = indices;
	GET_DEVICE()->CreateBuffer(&ibd, &iinitData, &m_IndexBuffer);

}

ID3D11Buffer* Shader::GetIndexBuffer()
{
	return m_IndexBuffer;
}

Shader* Shader::GetInstance()
{
	static Shader singletonShader;

	return &singletonShader;
}
