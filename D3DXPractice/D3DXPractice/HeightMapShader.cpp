#include "pch.h"
#include "HeightMapShader.h"
#include "Renderer.h"


HeightMapShader::HeightMapShader()
{
	CreateShader();
}


HeightMapShader::~HeightMapShader()
{
}

HeightMapShader* HeightMapShader::GetInstance()
{
	static HeightMapShader HMShader;

	return &HMShader;
}


ID3D11VertexShader* HeightMapShader::GetVertexShader() const
{
	return m_VertexShader;
}

ID3D11PixelShader* HeightMapShader::GetPixelShader() const
{
	return m_PixelShader;
}

ID3DBlob* HeightMapShader::GetVSBlob() const
{
	return m_VSBlob;
}

void HeightMapShader::CreateShader()
{
	CreateVertexShader();
	CreatePixelShader();
}

void HeightMapShader::CreateVertexShader()
{
	HRESULT hr = D3DX11CompileFromFile(L"HeightMapShader.fx", 0, 0,
		"VS", "vs_5_0", 0,
		0, 0,
		&m_VSBlob, &m_ErrorBlob, 0);

	hr = GET_DEVICE()->CreateVertexShader(
		m_VSBlob->GetBufferPointer(),
		m_VSBlob->GetBufferSize(),
		nullptr,
		&m_VertexShader);
}

void HeightMapShader::CreatePixelShader()
{
	// PixelShader
	ID3DBlob* PSBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT hr = D3DX11CompileFromFile(L"HeightMapShader.fx", 0, 0,
		"PS", "ps_5_0", 0,
		0, 0,
		&PSBlob, &errorBlob, 0);

	if (hr != S_OK)
	{
		char buffer[1000] = { 0, };
		sprintf_s(buffer, 1000, "last error : %d", GetLastError());
		MessageBoxA(NULL, buffer, "hr error", MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2);
	}

	hr = GET_DEVICE()->CreatePixelShader(
		PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(),
		nullptr, &m_PixelShader);

	PSBlob->Release();
}


