#include "pch.h"
#include "Node.h"
#include "Renderer.h"
#include "Shader.h"

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
