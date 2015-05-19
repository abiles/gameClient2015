#pragma once

#include <vector>

class Node
{
public:
	struct MyVertex
	{
		XMFLOAT3 pos;
		//XMFLOAT4 color;
	};

	Node();
	virtual ~Node();

	void addChild(Node* inputNode);
	
	virtual void Init();
	virtual void DrawByVertex();

	

protected:
	std::vector<Node*> m_HavingNodes;
	int defaultVectorSize = 10;
	
	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11InputLayout* m_InputLayout = nullptr;
	UINT		  m_Stride = sizeof(MyVertex);

};

