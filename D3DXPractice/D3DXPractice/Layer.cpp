#include "pch.h"
#include "Layer.h"


Layer::Layer()
{
}


Layer::~Layer()
{
}

void Layer::DrawByIndex()
{
	for (auto pNode : m_HavingNodes)
	{
		pNode->DrawByIndex();
	}
}
