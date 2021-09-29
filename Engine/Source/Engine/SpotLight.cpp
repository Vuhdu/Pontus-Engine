#include "pch.h"
#include "SpotLight.h"

#include "Camera.h"

void CSpotLight::SetTransform(const CU::Matrix4x4f& aTransform)
{
	myTransform = aTransform;
}

void CSpotLight::SetDirection(const CU::Vector3f& aDirection)
{
	myTransform.SetRotationRad(aDirection);
	assert(myShadowCamera && "No shadow camera was found on SpotLight!");
	myShadowCamera->SetRotation(aDirection);
}

void CSpotLight::SetPosition(const CU::Vector3f& aPosition)
{
	myTransform.SetPosition(aPosition);
	assert(myShadowCamera && "No shadow camera was found on SpotLight!");
	myShadowCamera->SetPosition(aPosition);
	CLight::SetPosition(aPosition);
}

void CSpotLight::SetRadius(const float anInnerRadius, const float anOuterRadius)
{
	myInnerRadius = anInnerRadius;
	myOuterRadius = anOuterRadius;
}

void CSpotLight::SetShadowCamera(CCamera* aShadowCamera)
{
	myShadowCamera = aShadowCamera;
}

void CSpotLight::SetShadowMap(CFullscreenTexture aFullscreenTexture)
{
	myShadowMap = aFullscreenTexture;
}
