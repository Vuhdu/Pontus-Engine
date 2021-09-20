#pragma once
#include "ParticleEmitter.h"

class CParticleEmitterInstance
{
public:
	CParticleEmitterInstance() = default;
	~CParticleEmitterInstance() = default;

	void Init(std::shared_ptr<CParticleEmitter> anEmitter);
	void Update(const float aDeltaTime, CU::Vector3f aCameraPosition);

	void SetTransform(const CU::Vector3f& aPosition, const CU::Vector3f& aRotation);
	void SetRotation(const CU::Vector3f& aRotation);
	void SetPosition(const CU::Vector3f& aPosition);

public:
	FORCEINLINE const std::shared_ptr<CParticleEmitter> GetEmitter() const { return myEmitter; };
	FORCEINLINE const CU::Matrix4x4f& GetTransform() const { return myTransform; };
	FORCEINLINE const std::vector<CParticleEmitter::SParticleVertex>& GetVertices() const { return myVertices; };

private:
	std::shared_ptr<CParticleEmitter> myEmitter;
	std::vector<CParticleEmitter::SParticleVertex> myVertices;
	CU::Matrix4x4f myTransform = {};

};

