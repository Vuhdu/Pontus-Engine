#include "pch.h"
#include "ParticleEmitter.h"

void CParticleEmitter::SetSettings(const SParticleSettings& someSettings)
{
	mySettings = someSettings;
}

void CParticleEmitter::SetData(const SParticleData& someData)
{
	myData = someData;
}