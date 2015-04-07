#include "pch.h"

struct MyVertex
{
	// 순서가 중요합니다. 

	XMFLOAT3 pos;
	XMFLOAT4 color;
};

struct ConstantBuffer
{
	XMMATRIX wvp;
};