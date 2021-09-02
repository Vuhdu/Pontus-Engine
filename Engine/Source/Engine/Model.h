#pragma once
#include "Mesh.h"

struct ID3D11InputLayout;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11Buffer;
struct ID3D11ShaderResourceView;

class CMaterial;

class CModel
{
public:
	struct SModelData
	{
		SMesh myMesh = {};

		std::array<ID3D11ShaderResourceView*, 3> myTexture = {};
	};

	CModel();
	~CModel();

	void Init(const SModelData& aModelData);

	SModelData GetModelData();

private:
	SModelData myModelData;

};

