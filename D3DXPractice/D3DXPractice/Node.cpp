#include "pch.h"
#include "Node.h"


Node::Node()
{
	havingNodes.reserve(defaultVectorSize);
}


Node::~Node()
{
	ReleaseCOM(m_VertexBuffer);
}

void Node::addChild(Node* inputNode)
{
	havingNodes.push_back(inputNode);
}
