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

	std::shared_ptr<CParticleEmitterInstance> GetParticleEmitter(const std::string& aParticleNameID);

protected:
	bool GetParticleEmitterInternal(const std::string& aParticleNameID, std::shared_ptr<CParticleEmitter> anOutEmitter);
	std::shared_ptr<CParticleEmitter> LoadParticleEmitter(
		const std::wstring& aTexturePath
	);

private:
	std::unordered_map<std::string, std::shared_ptr<CParticleEmitter>> myParticleEmitters;

	ID3D11Device* myDevice = nullptr;

};

