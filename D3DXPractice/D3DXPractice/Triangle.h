#pragma once
#include "Node.h"

class Triangle : public Node
{
public:
	Triangle();
	~Triangle();

	void CreateVertexBuffer() override;
	void Init() override;

private:

};

