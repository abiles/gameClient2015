#pragma once
class VertexManager
{
public:
	VertexManager();
	~VertexManager();

	HRESULT CreateVertexLayout();
	void CreateVertexBuffer();
	void CreateIndexBuffer();

	ID3D11InputLayout*  GetVertexLayout();
	ID3D11Buffer*		GetVertexBuffer();
	ID3D11Buffer*       GetIndexBuffer();

private:
	ID3D11InputLayout*   m_VertexLayout = nullptr;
	ID3D11Buffer*		 m_VertexBuffer = nullptr;
	ID3D11Buffer*		 m_IndexBuffer = nullptr;

};

