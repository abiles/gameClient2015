#pragma once

struct MySize
{
	int m_MaxWidth = 0;
	int m_MaxHeight = 0;
};

struct MyVertex
{
	// ������ �߿��մϴ�. 

	XMFLOAT3 pos;
	XMFLOAT4 color;
	XMFLOAT3 normal;
};

struct ConstantBuffer
{
	XMMATRIX wvp;
	XMMATRIX world;
	XMFLOAT4 lightDir;
	XMFLOAT4 lightColor;
};