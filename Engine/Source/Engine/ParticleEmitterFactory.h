#pragma once

class CParticleEmitter;
class CParticleEmitterInstance;

struct ID3D11Device;

class CParticleEmitterFactory
{
public:
	CParticleEmitterFactory() = default;
	~CParticleEmitterFactory() = default;

	void Init(ID3D11Device* aDevice);

	CParticleEmitterInstance* GetParticleEmitter(const std::string& aParticleNameID);

protected:
	bool GetParticleEmitterInternal(const std::string& aParticleNameID, CParticleEmitter*& anOutEmitter);
	CParticleEmitter* LoadParticleEmitter(
		const std::wstring& aTexturePath,
		const int aMaxAmount
	);

private:
	std::unordered_map<std::string, CParticleEmitter*> myParticleEmitters;

	ID3D11Device* myDevice = nullptr;

};

