#pragma once

struct ID3D11Buffer;
struct ID3D11VertexShader;
struct ID3D11GeometryShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;
struct ID3D11ShaderResourceView;

class CParticleEmitter
{
public:
	CParticleEmitter() = default;
	~CParticleEmitter() = default;

	enum class EmitterShape
	{
		Sphere,
		Box,
		eCount
	};

	struct SParticleSettings
	{
		float mySpawnRate = 1.0f;
		float mySpawnAngle = 0.0f;
		float myParticleLifetime = 3.0f;
		float myParticleSpeed = 1.5f;
		float myParticleStartSize = 0.0f;
		float myParticleEndSize = 1.0f;
		float myParticleEmissiveStrength = 5.0f;

		CU::Vector4f myParticleStartColor = { 1.0f, 0.25f, 0.0f, 1.0f };
		CU::Vector4f myParticleEndColor = { 1.0f, 0.25f, 0.0f, 0.0f };
	};

	struct SParticleData
	{
		unsigned int myNumberOfParticles = 0;
		unsigned int myStride = 0;
		unsigned int myOffset = 0;
		ID3D11Buffer* myParticleVertexBuffer = nullptr;
		ID3D11VertexShader* myParticleVertexShader = nullptr;
		ID3D11GeometryShader* myParticleGeometryShader = nullptr;
		ID3D11PixelShader* myParticlePixelShader = nullptr;
		UINT myPrimitiveTopology;
		ID3D11InputLayout* myInputLayout = nullptr;
		ID3D11ShaderResourceView* mySRV = nullptr;
	};

	struct SParticleVertex
	{
		CU::Vector4f myPosition;
		CU::Vector4f myVelocity;
		CU::Vector4f myColor;
		CU::Vector2f mySize;
		float myLifetime;
		float myDistanceToCamera;
		float myEmissiveStrength;
	};

public:
	void SetSettings(const SParticleSettings& someSettings);
	void SetData(const SParticleData& someData);

	const SParticleSettings& GetSettings() const { return mySettings; };
	const SParticleData& GetData() const { return myData; };

private:
	SParticleSettings mySettings;
	SParticleData myData;

	std::wstring myFilePath;

};

