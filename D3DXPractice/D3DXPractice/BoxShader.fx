cbuffer cbPerObject
{
	float4x4 gWVP;
	float4x4 gWorld;
};

struct    VertexIn
{
	float3   pos : POSITION;
	float4   color : COLOR;
};

struct    VertexOut
{
	float4   pos : SV_POSITION;
	float4   color : COLOR;
};



VertexOut  VS(VertexIn vIn)
{
	VertexOut	 vOut;
	vOut.pos = mul(float4(vIn.pos, 1.0f), gWVP);
	vOut.color = vIn.color;

	return vOut;

}

float4   PS(VertexOut vOut) : SV_TARGET
{
	return vOut.color;

}
