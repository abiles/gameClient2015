#pragma once

#include <vector>

class Node
{
public:
	struct MyVertex
	{
		XMFLOAT3 m_Pos;
	};

	struct ConstantBuffer
	{
		XMFLOAT4X4 m_WVP;
	};

	Node();
	virtual ~Node();

	void addChild(Node* inputNode);
	
	virtual void Init();
	virtual void InitMatrix();
	virtual void DrawByVertex();
	virtual void DrawByIndex();
	virtual void CreateVertexBuffer();
	virtual void CreateIndexBuffer();
	virtual void CreateConstantBuffer();
	virtual void CreateVertexLayOut();
	virtual void LoadTexture();


	

protected:
	std::vector<Node*> m_HavingNodes;
	int defaultVectorSize = 10;
	
	ID3D11Buffer*		m_VertexBuffer = nullptr;
	ID3D11Buffer*		m_IndexBuffer = nullptr;
	ID3D11InputLayout*  m_InputLayout = nullptr;
	UINT				m_Stride = sizeof(MyVertex);
	
	XMFLOAT4X4			m_World;
	XMFLOAT4X4			m_View;
	XMFLOAT4X4			m_Projection;

	

};

