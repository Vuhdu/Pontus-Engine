#include "pch.h"
#include "SpotLight.h"

void CSpotLight::SetDirection(const CU::Vector3f& aDirection)
{
	myDirection = aDirection;
}

void CSpotLight::SetRadius(const float anInnerRadius, const float anOuterRadius)
{
	myInnerRadius = anInnerRadius;
	myOuterRadius = anOuterRadius;
}
