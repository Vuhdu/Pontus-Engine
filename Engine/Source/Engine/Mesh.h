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

	float r0, g0, b0, a0;		// colors0
	float r1, g1, b1, a1;		// colors1
	float r2, g2, b2, a2;		// colors2
	float r3, g3, b3, a3;		// colors3

	float u0, v0;				// uv0
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
	UINT myStride;
	UINT myOffset;

	std::vector<SVertex> myVertices;
	std::vector<UINT32> myIndices;

	CMaterial* myMaterial;

	ID3D11Buffer* myVertexBuffer;
	ID3D11Buffer* myIndexBuffer;

	ID3D11VertexShader* myVertexShader;
	ID3D11PixelShader* myPixelShader;
	ID3D11InputLayout* myInputLayout;

	unsigned int myPrimitiveTopology;

	ID3D11ShaderResourceView* myTexture = nullptr;

	virtual ~SMesh()
	{
	};
};
