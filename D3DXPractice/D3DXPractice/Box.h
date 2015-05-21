#pragma once
#include "Node.h"
class Box :
	public Node
{
public:
	Box();
	~Box();

	struct BoxVertex 
	{
		XMFLOAT3 m_Pos;
		XMFLOAT4 m_Color;
	};

	void Init() override;
	void CreateVertexLayOut() override;
	void CreateVertexBuffer() override;
	void CreateIndexBuffer() override;
	void CreateConstantBuffer() override;

	void DrawByIndex() override;


private:
	
};

