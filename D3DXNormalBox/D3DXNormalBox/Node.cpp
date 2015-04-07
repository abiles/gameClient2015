#include "pch.h"
#include "Node.h"
#include "WindowManager.h"


Node::Node()
{
}


Node::~Node()
{
}

bool Node::init()
{
	initMatrix();

	return true;
}

void Node::initMatrix()
{
	m_World = XMMatrixIdentity();
	XMVECTOR pos = XMVectorSet(0.0f, 0.0f, -5.0f, 1.0f);;
	XMVECTOR target = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	m_View = XMMatrixLookAtLH(pos, target, up);

	MySize wndSize = WindowManager::getInstance()->getWndSize();

	m_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV2,
		wndSize.m_MaxWidth / (float)wndSize.m_MaxHeight,
		0.01f, 100.0f);  // near, far plane; 
}

HRESULT Node::createVertexBuffer()
{
	HRESULT hr = S_OK;
	return hr;
}

HRESULT Node::createIndexBuffer()
{
	HRESULT hr = S_OK;
	return hr;
}

HRESULT Node::createConstantBuffer()
{
	HRESULT hr = S_OK;
	return hr;

}

ID3D11Buffer* Node::getVertexBuffer()
{
	return m_pVertexBuffer;
}

ID3D11Buffer* Node::getIndexBuffer()
{
	return m_pIndexBuffer;
}
