#pragma once

#include "D3DXApp.h"


struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

class BoxApp : public D3DXApp
{
public:
	BoxApp(HINSTANCE hInstance);
	~BoxApp();


	bool init();
	void onResize();
	void updateScene(float dt);
	void drawScene();

	void onMouseDown(WPARAM btnState, int x, int y);
	void onMouseUp(WPARAM btnState, int x, int y);
	void onMouseMove(WPARAM btnState, int x, int y);

private:
	void buildGeometryBuffers();
	void buildFX();
	void buildVertexLayout();

private:
	ID3D11Buffer* m_BoxVB = nullptr;
	ID3D11Buffer* m_BoxIB = nullptr;
	
	ID3DX11Effect* m_FX = nullptr;
	ID3DX11EffectTechnique* m_Tech = nullptr;
	ID3DX11EffectMatrixVariable* m_FXWorldViewProj = nullptr;
	ID3D11InputLayout* m_InputLayout = nullptr;

	XMFLOAT4X4 m_World;
	XMFLOAT4X4 m_View;
	XMFLOAT4X4 m_Proj;

	float m_Theta = 1.5f * MathHelper::Pi;
	float m_Phi = 0.25f * MathHelper::Pi;
	float m_Radiuos = 5.0f;

	POINT m_LastMousePoint;
};

