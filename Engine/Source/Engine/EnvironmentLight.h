#pragma once

struct ID3D11ShaderResourceView;

class CEnvironmentLight
{
public:
	CEnvironmentLight() = delete;
	CEnvironmentLight(ID3D11ShaderResourceView* aSRV) : myCubeMap(aSRV) { };
	~CEnvironmentLight();

	void SetDirection(const CU::Vector3f& aDirection);
	void SetColor(const CU::Vector3f& aColor);

public:
	ID3D11ShaderResourceView* GetCubeMap() { return myCubeMap; }
	ID3D11ShaderResourceView* const* GetCubeMapConstPtr() { return &myCubeMap; }
	const CU::Vector4f& GetDirection() const { return myDirection; }
	const CU::Vector4f& GetColor() const { return myColor; }


private:
	ID3D11ShaderResourceView* myCubeMap = nullptr;
	CU::Vector4f myDirection = { };
	CU::Vector4f myColor = { };

};