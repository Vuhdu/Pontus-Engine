#pragma once

struct ID3D11VertexShader;
struct ID3D11PixelShader;

class CMaterial
{
public:
	CMaterial() = default;
	~CMaterial() = default;

	bool Init(unsigned int anID, ID3D11VertexShader* aVertexShader, ID3D11PixelShader* aPixelShader);

private:
	ID3D11VertexShader* myVertexShader = nullptr;
	ID3D11PixelShader* myPixelShader = nullptr;

	unsigned int myID = -1;

};

