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
};

struct ConstantBuffer
{
	XMMATRIX wvp;
};