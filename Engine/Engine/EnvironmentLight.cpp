#include "stdafx.h"
#include "EnvironmentLight.h"

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
