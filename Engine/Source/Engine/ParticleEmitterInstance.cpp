#include "pch.h"
#include "ParticleEmitterInstance.h"

#include <d3d11.h>

void CParticleEmitterInstance::Init(std::shared_ptr<CParticleEmitter> anEmitter)
{
	myEmitter = anEmitter;
}

void CParticleEmitterInstance::Update(const float aDeltaTime, CU::Vector3f aCameraPosition)
{
	CParticleEmitter::SParticleData data = myEmitter->GetData();
	CParticleEmitter::SParticleSettings settings = myEmitter->GetSettings();

	int start = myVertices.size();
	for (int i = start; i >= 0; --i)
	{
		// update

		myVertices[i].myPosition;
		myVertices[i].myVelocity;
		myVertices[i].mySize;
		myVertices[i].myLifetime;
		myVertices[i].myEmissiveStrength;
		myVertices[i].myColor;
	}

	for (unsigned int firstIndex = 0; firstIndex < myVertices.size(); firstIndex++)
	{
		for (unsigned int secondIndex = 0; secondIndex < myVertices.size(); secondIndex++)
		{
			CParticleEmitter::SParticleVertex& firstParticle= myVertices[firstIndex];
			CParticleEmitter::SParticleVertex& secondParticle = myVertices[secondIndex];

			if (firstParticle.myDistanceToCamera < secondParticle.myDistanceToCamera)
			{
				std::swap(firstParticle, secondParticle);
			}
		}
	}
}

void CParticleEmitterInstance::SetTransform(const CU::Vector3f& aPosition, const CU::Vector3f& aRotation)
{
	SetPosition(aPosition);
	SetRotation(aRotation);
}

void CParticleEmitterInstance::SetRotation(const CU::Vector3f& aRotation)
{
	CU::Matrix4x4f translation;
	translation(4, 1) = myTransform(4, 1);
	translation(4, 2) = myTransform(4, 2);
	translation(4, 3) = myTransform(4, 3);

	const CU::Matrix4x4f newMatrix =
		CU::Matrix4x4f::CreateRotationAroundZ(aRotation.z) *
		CU::Matrix4x4f::CreateRotationAroundX(aRotation.x) *
		CU::Matrix4x4f::CreateRotationAroundY(aRotation.y) *
		translation;

	myTransform = newMatrix;
}

void CParticleEmitterInstance::SetPosition(const CU::Vector3f& aPosition)
{
	myTransform(4, 1) = aPosition.x;
	myTransform(4, 2) = aPosition.y;
	myTransform(4, 3) = aPosition.z;
	myTransform(4, 4) = 1.0f;
}
