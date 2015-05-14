#pragma once

#include <vector>

class Node
{
public:
	Node();
	virtual ~Node();

	void addChild(Node* inputNode);

protected:
	std::vector<Node*> havingNodes;
	int defaultVectorSize = 10;
	
};

