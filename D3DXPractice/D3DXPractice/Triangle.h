#pragma once
#include "Node.h"

class Triangle : public Node
{
public:
	Triangle();
	~Triangle();

	void CreateVertexBuffer();
	void Init() override;
	//void DrawByVertex() override;

private:

};

