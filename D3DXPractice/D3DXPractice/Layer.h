#pragma once
#include "Node.h"
class Layer : public Node
{
public:
	Layer();
	virtual ~Layer();

	void DrawByIndex() override;
};

