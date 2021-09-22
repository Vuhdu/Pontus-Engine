#pragma once

class CMaterial;

struct ID3D11InputLayout;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11Buffer;
struct ID3D11ShaderResourceView;

struct SVertex 
{
	float x, y, z, w;			// positions

	float r, g, b, a;			// colors0
	float r1, g1, b1, a1;		// colors1
	float r2, g2, b2, a2;		// colors2
	float r3, g3, b3, a3;		// colors3

	float u, v;					// uv0
	float u1, v1;				// uv1
	float u2, v2;				// uv2
	float u3, v3;				// uv3

	float nx, ny, nz, nw;		// normals
	float tx, ty, tz, tw;		// tangent
	float bx, by, bz, bw;		// binormal
};

struct SMesh
{
	UINT myNumVertices = 0;
	UINT myNumIndices = 0;
	UINT myStride = 0;
	UINT myOffset = 0;

	std::vector<SVertex> myVertices;
	std::vector<UINT32> myIndices;

	CMaterial* myMaterial = nullptr;

	ID3D11Buffer* myVertexBuffer = nullptr;
	ID3D11Buffer* myIndexBuffer = nullptr;

	ID3D11VertexShader* myVertexShader = nullptr;
	ID3D11PixelShader* myPixelShader = nullptr;
	ID3D11InputLayout* myInputLayout = nullptr;

	unsigned int myPrimitiveTopology = 0;

	ID3D11ShaderResourceView* myTexture = nullptr;

	virtual ~SMesh()
	{
	};
};
