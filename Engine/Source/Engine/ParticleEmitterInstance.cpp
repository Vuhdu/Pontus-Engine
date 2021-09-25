#include "pch.h"
#include "ParticleEmitterInstance.h"

#include "Random.h"

#include <d3d11.h>

void CParticleEmitterInstance::Init(CParticleEmitter* anEmitter)
{
	myEmitter = anEmitter;
}

void CParticleEmitterInstance::Update(const float aDeltaTime, CU::Vector3f aCameraPosition)
{
	CParticleEmitter::SParticleData data = myEmitter->GetData();
	CParticleEmitter::SParticleSettings settings = myEmitter->GetSettings();

	mySpawnTimer += aDeltaTime;
	if (mySpawnTimer >= settings.mySpawnRate)
	{
		InstantiateParticle(settings);
		mySpawnTimer = 0.0f;
	}

	int start = myVertices.size() - 1;
	for (int i = start; i >= 0; --i)
	{
		myVertices[i].myLifetime += aDeltaTime;

		const float ratio = myVertices[i].myLifetime / settings.myParticleLifetime;

		if (ratio >= 1.0f)
		{
			std::swap(myVertices[i], myVertices.back());
			myVertices.pop_back();
			continue;
		}

		const float size = CU::Lerp(settings.myParticleStartSize, settings.myParticleEndSize, ratio);
		const float red = CU::Lerp(settings.myParticleStartColor.x, settings.myParticleEndColor.x, ratio);
		const float green = CU::Lerp(settings.myParticleStartColor.y, settings.myParticleEndColor.y, ratio);
		const float blue = CU::Lerp(settings.myParticleStartColor.z, settings.myParticleEndColor.z, ratio);
		const float alpha = CU::Lerp(settings.myParticleStartColor.w, settings.myParticleEndColor.w, ratio);

		myVertices[i].myPosition += myVertices[i].myVelocity * settings.myParticleSpeed * aDeltaTime;
		myVertices[i].mySize = {
			size, size
		};
		myVertices[i].myColor = {
			red, green, blue, alpha
		};
		myVertices[i].myDistanceToCamera = CU::Vector4f{
			aCameraPosition.x,
			aCameraPosition.y,
			aCameraPosition.z,
			1.0f
		}.LengthSqr();
	}

	for (unsigned int firstIndex = 0; firstIndex < myVertices.size(); firstIndex++)
	{
		for (unsigned int secondIndex = 0; secondIndex < myVertices.size(); secondIndex++)
		{
			CParticleEmitter::SParticleVertex& firstParticle = myVertices[firstIndex];
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

void CParticleEmitterInstance::InstantiateParticle(CParticleEmitter::SParticleSettings& someSettings)
{
	CParticleEmitter::SParticleVertex particle;
	particle.myColor = someSettings.myParticleStartColor;
	particle.myLifetime = 0.0f;
	particle.mySize = { someSettings.myParticleStartSize, someSettings.myParticleStartSize };
	particle.myPosition = { myTransform(4, 1), myTransform(4, 2), myTransform(4, 3), 1.0f };
	particle.myEmissiveStrength = someSettings.myParticleEmissiveStrength;
	
	particle.myVelocity = CU::Vector4f{
		Random::RandomFloat(-180.0f, 180.0f),
		Random::RandomFloat(-180.0f, 180.0f),
		Random::RandomFloat(-180.0f, 180.0f),
		0.0f
	}.GetNormalized();

	myVertices.push_back(particle);
}
