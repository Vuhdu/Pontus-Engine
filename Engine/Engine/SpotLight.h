#pragma once
#include "Light.h"

class CSpotLight : public CLight
{
public:
	CSpotLight() = default;
	~CSpotLight() = default;

	void SetDirection(const CU::Vector3f& aDirection);
	void SetRadius(const float anInnerRadius, const float anOuterRadius);

public:
	const CU::Vector3f& GetDirection() const { return myDirection; };
	const float GetInnerRadius() const { return myInnerRadius; };
	const float GetOuterRadius() const { return myOuterRadius; };

private:
	CU::Vector3f myDirection = { 0.0f, 0.0f, 0.0f };
	float myInnerRadius = 1.0f;
	float myOuterRadius = 10.0f;


};

