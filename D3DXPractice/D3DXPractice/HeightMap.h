#pragma once
#include "Node.h"

class HeightMap : public Node
{
public:

	struct HeightVertex : public MyVertex
	{
		XMFLOAT4 m_Color;
		XMFLOAT2 m_Tex;
	};


	HeightMap();
	virtual ~HeightMap();

	static const int m_VertexCount = 257;
	static const int m_NumOfVertecies = 66049;

	void Init() override;
	void LoadHeightMap();
	void CreateVertexLayOut() override;
	void CreateVertexBuffer() override;
	void CreateIndexBuffer() override;
	void CreateConstantBuffer() override;
	void LoadTexture() override;

	void DrawByIndex() override;
	void DrawByVertex() override;

private:
	int              m_IndexSize = 0;

	ID3D11Buffer*    m_VertexBuffer = nullptr;
	ID3D11Buffer*    m_IndexBuffer = nullptr;
	ID3D11Buffer*    m_HeightConstantBuffer = nullptr;

	std::vector<int> m_HeightMap;

	ID3D11ShaderResourceView* m_TextureRV = nullptr;
	ID3D11SamplerState*		  m_SamplerLinear = nullptr;
};

