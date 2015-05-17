#include "pch.h"
#include "Triangle.h"
#include "Renderer.h"


Triangle::Triangle()
{
	CreateVertexBuffer();
}


Triangle::~Triangle()
{
	
}

void Triangle::CreateVertexBuffer()
{
	MyVertex	 vertices[] =
	{
		{ XMFLOAT3(0.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(0.5f, 0.5f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-0.5f, -0.5f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
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

	UINT stride = sizeof(MyVertex);
	UINT offset = 0;
	GET_DEVICECONTEXT()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
}
