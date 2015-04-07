#include "pch.h"
#include "Box.h"
#include "DeviceManager.h"

Box::Box()
{
}


Box::~Box()
{
}

bool Box::init()
{
	if (!Node::init())
		return false;

	HRESULT hr = createVertexBuffer();

	if (FAILED(hr))
		return false;

	hr = createIndexBuffer();

	if (FAILED(hr))
		return false;

	hr = createConstantBuffer();

	if (FAILED(hr))
		return false;

	return true;
}

HRESULT Box::createVertexBuffer()
{
	MyVertex vertices[] =
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

	D3D11_BUFFER_DESC bufferDecs;
	ZeroMemory(&bufferDecs, sizeof(bufferDecs));
	bufferDecs.ByteWidth = sizeof(vertices);
	bufferDecs.Usage = D3D11_USAGE_DEFAULT;
	bufferDecs.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDecs.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = vertices;

	ID3D11Device* device = DeviceManager::getInstance()->getD3D11Device();
	HRESULT hr = device->CreateBuffer(&bufferDecs, &initData, &m_pVertexBuffer);

	return hr;
}

HRESULT Box::createIndexBuffer()
{
	UINT indices[] =
	{

		// 그릴 때 화면에 보이게 하려면 항상 
		// 시계방향으로 해야됨
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

	D3D11_BUFFER_DESC indexBufferDecs;
	ZeroMemory(&indexBufferDecs, sizeof(indexBufferDecs));
	indexBufferDecs.ByteWidth = sizeof(indices);
	indexBufferDecs.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDecs.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDecs.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = indices;

	ID3D11Device* device = DeviceManager::getInstance()->getD3D11Device();
	HRESULT hr = device->CreateBuffer(&indexBufferDecs, &initData, &m_pIndexBuffer);

	return hr;
}

HRESULT Box::createConstantBuffer()
{
	D3D11_BUFFER_DESC 	cbd;
	ZeroMemory(&cbd, sizeof(cbd));
	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.ByteWidth = sizeof(ConstantBuffer);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = 0;

	ID3D11Device* device = DeviceManager::getInstance()->getD3D11Device();
	HRESULT hr = device->CreateBuffer(&cbd, NULL, &m_pConstantBuffer);

	return hr;
}

void Box::calculateMatrix(float deltaTime)
{
	XMMATRIX mat = XMMatrixRotationY(deltaTime);
	//mat *= XMMatrixRotationX(-deltaTime);
	m_World = mat;

	XMMATRIX wvp = m_World * m_View * m_Projection;
	ConstantBuffer cb;
	cb.wvp = XMMatrixTranspose(wvp);

	ID3D11DeviceContext* context = DeviceManager::getInstance()->getD3D11DeviceContext();
	context->UpdateSubresource(m_pConstantBuffer, 0, 0, &cb, 0, 0);
	context->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);


}
