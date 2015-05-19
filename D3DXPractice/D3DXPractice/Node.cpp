#include "pch.h"
#include "Node.h"
#include "Renderer.h"
#include "Shader.h"
#include "Application.h"


Node::Node()
{
	m_HavingNodes.reserve(10);
}


Node::~Node()
{
	ReleaseCOM(m_VertexBuffer);
}

void Node::addChild(Node* inputNode)
{
	m_HavingNodes.push_back(inputNode);
}


void Node::Init()
{
	// layout
	/*D3D11_INPUT_ELEMENT_DESC	 layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },

	};*/

	D3D11_INPUT_ELEMENT_DESC	 layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },

	};

	UINT   numElements = ARRAYSIZE(layout);
	ID3DBlob* vsBlob = GET_TRIANGLE_SHADER()->GetVSBlob();

	if (vsBlob)
	{
		GET_DEVICE()->CreateInputLayout(layout, numElements, vsBlob->GetBufferPointer(),
			vsBlob->GetBufferSize(), &m_InputLayout);
	}
	
}

void Node::DrawByVertex()
{
	GET_DEVICECONTEXT()->IASetInputLayout(m_InputLayout);
	GET_DEVICECONTEXT()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT stride = sizeof(MyVertex);
	UINT offset = 0;
	GET_DEVICECONTEXT()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
	
	GET_DEVICECONTEXT()->VSSetShader(GET_TRIANGLE_SHADER()->GetVertexShader(), NULL, 0);
	GET_DEVICECONTEXT()->PSSetShader(GET_TRIANGLE_SHADER()->GetPixelShader(), NULL, 0);
	GET_DEVICECONTEXT()->Draw(3, 0);
}

void Node::CreateVertexBuffer()
{

}

void Node::CreateConstantBuffer()
{

}

void Node::InitMatrix()
{
	// World 행렬 초기화
	XMStoreFloat4x4(&m_World, XMMatrixIdentity());

	// View 행렬 구성
	XMVECTOR 	pos = XMVectorSet(0.0f, 0.0f, -5.0f, 1.0f);
	XMVECTOR 	target = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR 	up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMStoreFloat4x4(&m_View, XMMatrixLookAtLH(pos, target, up));

	auto tmpXMMatrix = XMMatrixPerspectiveFovLH(
		XM_PIDIV2,  	// pi
		Application::GetInstance()->GetWndWidth() / (FLOAT) Application::GetInstance()->GetWndHeight(), // aspect ratio
		0.01f, 100.0f);  	// near plane, far plane
	
	XMStoreFloat4x4(&m_Projection, tmpXMMatrix);



}

void Node::LoadTexture()
{

}

void Node::CreateIndexBuffer()
{

}

void Node::CreateVertexLayOut()
{

}

void Node::DrawByIndex()
{

}
