#pragma once
#include "Node.h"


class Box :
	public Node
{
public:
	Box();
	~Box();

	virtual bool    init();
	virtual HRESULT createVertexBuffer();
	virtual HRESULT createIndexBuffer();
	virtual HRESULT createConstantBuffer();

	void			calculateMatrix(float deltaTime);
private:
};

