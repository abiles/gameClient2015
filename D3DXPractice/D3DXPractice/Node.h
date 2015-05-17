#pragma once

#include <vector>

class Node
{
public:
	struct MyVertex
	{
		XMFLOAT3 pos;
		XMFLOAT4 color;
	};

	Node();
	virtual ~Node();

	void addChild(Node* inputNode);

protected:
	std::vector<Node*> havingNodes;
	int defaultVectorSize = 10;
	
	ID3D11Buffer* m_VertexBuffer = nullptr;

};

