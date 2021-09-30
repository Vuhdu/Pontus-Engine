#include "pch.h"
#include "Light.h"

void CLight::SetPosition(const CU::Vector3f& aPosition)
{
	myTransform.SetPosition(aPosition);
}

void CLight::SetColor(const CU::Vector3f& aColor)
{
	myColor = aColor;
}

void CLight::SetIntensity(float anIntensity)
{
	myIntensity = anIntensity;
}

void CLight::SetRange(float aRange)
{
	myRange = aRange;
}
