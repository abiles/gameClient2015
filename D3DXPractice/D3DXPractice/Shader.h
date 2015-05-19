#pragma once

#define  GET_TRIANGLE_SHADER() Shader::GetInstance()

struct MyVertex
{
	XMFLOAT3 pos;
	XMFLOAT4 color;
};

class Shader
{
public:
	Shader();
	~Shader();

	static Shader* GetInstance();

	void CreateShader();
	void CreateVertexBuffer();
	void CreateIndexBuffer();

	ID3D11VertexShader* GetVertexShader();
	ID3D11InputLayout*  GetVertexLayout();
	ID3D11PixelShader*  GetPixelShader();
	ID3D11Buffer*		GetVertexBuffer();
	ID3D11Buffer*       GetIndexBuffer();
	ID3DBlob*           GetVSBlob() const;

private:
	void CreateVertexShader();
	void CreatePixelShader();
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11InputLayout*  m_VertexLayout = nullptr;
	ID3D11PixelShader*  m_PixelShader = nullptr;


	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11Buffer* m_IndexBuffer = nullptr;

	ID3DBlob* m_VSBlob = nullptr;
	ID3DBlob* m_ErrorBlob = nullptr;



};

