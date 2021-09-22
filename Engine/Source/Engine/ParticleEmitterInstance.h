#pragma once
#include "ParticleEmitter.h"

class CParticleEmitterInstance
{
public:
	CParticleEmitterInstance() = default;
	~CParticleEmitterInstance() = default;

	void Init(CParticleEmitter* anEmitter);
	void Update(const float aDeltaTime, CU::Vector3f aCameraPosition);

	void SetTransform(const CU::Vector3f& aPosition, const CU::Vector3f& aRotation);
	void SetRotation(const CU::Vector3f& aRotation);
	void SetPosition(const CU::Vector3f& aPosition);

public:
	const CParticleEmitter* GetEmitter() const { return myEmitter; };
	const CU::Matrix4x4f& GetTransform() const { return myTransform; };
	const std::vector<CParticleEmitter::SParticleVertex>& GetVertices() const { return myVertices; };

private:
	void InstantiateParticle(CParticleEmitter::SParticleSettings& someSettings);

private:
	CParticleEmitter* myEmitter;
	std::vector<CParticleEmitter::SParticleVertex> myVertices;
	CU::Matrix4x4f myTransform = {};

	float mySpawnTimer = 0.0f;

};

