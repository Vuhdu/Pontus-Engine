#pragma once

struct ID3D11InputLayout;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11Buffer;
struct ID3D11ShaderResourceView;

//using D3D11_PRIMITIVE_TOPOLOGY = unsigned int;

class CModel
{
public:
	struct SModelData
	{
		unsigned int myPrimitiveTopology = {};
		ID3D11InputLayout* myInputLayout = nullptr;
		ID3D11Buffer* myVertexBuffer = nullptr;
		UINT myStride = 0;
		UINT myOffset = 0;

		ID3D11VertexShader* myVertexShader = nullptr;
		ID3D11PixelShader* myPixelShader = nullptr;
		UINT myNumberOfVertecies = 0;
		UINT myNumberOfIndices = 0;
		ID3D11Buffer* myIndexBuffer = nullptr;

		std::array<ID3D11ShaderResourceView*, 3> myTexture = {};
	};

	CModel();
	~CModel();

	void Init(const SModelData& aModelData);

	SModelData GetModelData();

private:
	SModelData myModelData;

};

