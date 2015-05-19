#pragma once

#define GET_HM_SHADER HeightMapShader::GetInstance

class HeightMapShader
{
public:
	static HeightMapShader* GetInstance();

	ID3D11VertexShader* GetVertexShader() const;
	ID3D11PixelShader*  GetPixelShader() const ;
	ID3DBlob*           GetVSBlob() const;

private:
	HeightMapShader();
	~HeightMapShader();
	
	void CreateShader();
	void CreateVertexShader();
	void CreatePixelShader();

private:
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader*  m_PixelShader = nullptr;

	ID3DBlob* m_VSBlob = nullptr;
	ID3DBlob* m_ErrorBlob = nullptr;
};

