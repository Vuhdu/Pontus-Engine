#include "pch.h"
#include "EnvironmentLight.h"
#include "Camera.h"

CEnvironmentLight::~CEnvironmentLight()
{
}

void CEnvironmentLight::SetDirection(const CU::Vector3f& aDirection)
{
	myDirection.x = aDirection.x;
	myDirection.y = aDirection.y;
	myDirection.z = aDirection.z;
	myDirection.w = 0.0f;
}

void CEnvironmentLight::SetColor(const CU::Vector3f& aColor)
{
	myColor.x = aColor.x;
	myColor.y = aColor.y;
	myColor.z = aColor.z;
	myColor.w = 0.8f;
}

void CEnvironmentLight::SetIntensity(const float& anIntensity)
{
	myColor.w = anIntensity;
}

void CEnvironmentLight::SetShadowCamera(CCamera* aCamera)
{
	myShadowCamera = aCamera;
}

void CEnvironmentLight::SetShouldCastShadows(bool aShouldCastShadows)
{
	myIsCastingShadows = aShouldCastShadows;
}

void CEnvironmentLight::SetShadowMap(const CFullscreenTexture aShadowMap)
{
	myShadowMap = aShadowMap;
}
