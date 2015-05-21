#include "pch.h"
#include "Box.h"
#include "Renderer.h"
#include "Shader.h"
#include "Director.h"
#include "Camera.h"

Box::Box()
{
}


Box::~Box()
{
}

void Box::Init()
{
	Node::InitMatrix();

	CreateVertexLayOut();
	CreateVertexBuffer();
	CreateIndexBuffer();
	CreateConstantBuffer();
}

void Box::CreateVertexLayOut()
{
	D3D11_INPUT_ELEMENT_DESC	 layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },

	};

	UINT   numElements = ARRAYSIZE(layout);
	ID3DBlob* vsBlob = GET_BOX_SHADER()->GetVSBlob();

	if (vsBlob)
	{
		GET_DEVICE()->CreateInputLayout(layout, numElements, vsBlob->GetBufferPointer(),
			vsBlob->GetBufferSize(), &m_InputLayout);
	}
}

void Box::CreateVertexBuffer()
{
	BoxVertex vertices[] =
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

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(vertices);
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;
	HRESULT hr = GET_DEVICE()->CreateBuffer(&bd, &InitData, &m_VertexBuffer);
}

void Box::CreateIndexBuffer()
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

	D3D11_BUFFER_DESC ibd;
	ZeroMemory(&ibd, sizeof(ibd));
	ibd.ByteWidth = sizeof(indices);
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;
	ZeroMemory(&iinitData, sizeof(iinitData));
	iinitData.pSysMem = indices;
	HRESULT hr = GET_DEVICE()->CreateBuffer(&ibd, &iinitData, &m_IndexBuffer);

}

void Box::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC 	cbd;
	ZeroMemory(&cbd, sizeof(cbd));
	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.ByteWidth = sizeof(ConstantBuffer);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = 0;
	GET_DEVICE()->CreateBuffer(&cbd, NULL, &m_ConstantBuffer);
}

void Box::DrawByIndex()
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
	GET_DEVICECONTEXT()->VSSetShader(GET_BOX_SHADER()->GetVertexShader(), NULL, 0);
	GET_DEVICECONTEXT()->PSSetShader(GET_BOX_SHADER()->GetPixelShader(), NULL, 0);


	UINT stride = sizeof(BoxVertex);
	UINT offset = 0;
	GET_DEVICECONTEXT()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
	GET_DEVICECONTEXT()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	GET_DEVICECONTEXT()->DrawIndexed(36, 0, 0);
}
