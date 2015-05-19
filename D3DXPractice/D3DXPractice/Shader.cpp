#include "pch.h"
#include "Shader.h"
#include "Director.h"
#include "Renderer.h"


Shader::Shader()
{
	CreateShader();
}


Shader::~Shader()
{
	ReleaseCOM(m_IndexBuffer);
	ReleaseCOM(m_VertexShader);
	ReleaseCOM(m_PixelShader);
	ReleaseCOM(m_VSBlob);
	ReleaseCOM(m_ErrorBlob);

}

void Shader::CreateShader()
{
	CreateVertexShader();
	CreatePixelShader();
}

void Shader::CreateVertexShader()
{

	HRESULT hr = D3DX11CompileFromFile(L"DefaultShader.fx", 0, 0,
		"VS", "vs_5_0", 0,
		0, 0,
		&m_VSBlob, &m_ErrorBlob, 0);

	hr = GET_DEVICE()->CreateVertexShader(
		m_VSBlob->GetBufferPointer(),
		m_VSBlob->GetBufferSize(),
		nullptr,
		&m_VertexShader);

}

void Shader::CreatePixelShader()
{
	// PixelShader
	ID3DBlob* PSBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	D3DX11CompileFromFile(L"DefaultShader.fx", 0, 0,
		"PS", "ps_5_0", 0,
		0, 0,
		&PSBlob, &errorBlob, 0);

	HRESULT hr = GET_DEVICE()->CreatePixelShader(
		PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(),
		nullptr, &m_PixelShader);

	PSBlob->Release();
}


ID3D11VertexShader* Shader::GetVertexShader()
{
	return m_VertexShader;
}


ID3D11PixelShader* Shader::GetPixelShader()
{
	return m_PixelShader;
}


Shader* Shader::GetInstance()
{
	static Shader singletonShader;

	return &singletonShader;
}

ID3DBlob* Shader::GetVSBlob() const
{
	return m_VSBlob;
}
