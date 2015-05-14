#include "pch.h"
#include "Node.h"


Node::Node()
{
	havingNodes.reserve(defaultVectorSize);
}


Node::~Node()
{
}

void Node::addChild(Node* inputNode)
{
	havingNodes.push_back(inputNode);
}
