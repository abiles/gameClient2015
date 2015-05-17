#include "pch.h"
#include "VertexManager.h"
#include "Renderer.h"


VertexManager::VertexManager()
{
}


VertexManager::~VertexManager()
{
}

ID3D11InputLayout* VertexManager::GetVertexLayout()
{

}

HRESULT VertexManager::CreateVertexLayout()
{
	D3D11_INPUT_ELEMENT_DESC	 layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },

	};

	UINT   numElements = ARRAYSIZE(layout);

	HRESULT hr = GET_DEVICE()->CreateInputLayout(layout, numElements, )
}
