#pragma once

class Node
{
public:
	Node();
	virtual ~Node();

	virtual bool init();
	virtual void initMatrix();
	virtual HRESULT createVertexBuffer();
	virtual HRESULT createIndexBuffer();
	virtual HRESULT createConstantBuffer();
	ID3D11Buffer*   getVertexBuffer();
	ID3D11Buffer*   getIndexBuffer();



protected:
	// buffer ����
	ID3D11Buffer*              m_pVertexBuffer = nullptr;
	ID3D11Buffer*			   m_pIndexBuffer = nullptr;
	ID3D11Buffer*              m_pConstantBuffer = nullptr;

	// ��ǥ ��ȯ ����
	XMMATRIX m_World;
	XMMATRIX m_View;
	XMMATRIX m_Projection;
};
