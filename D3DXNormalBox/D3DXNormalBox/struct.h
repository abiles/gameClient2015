#include "pch.h"

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