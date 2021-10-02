#pragma once
#include "Light.h"
#include "FullscreenTexture.h"

class CCamera;

class CSpotLight : public CLight
{
public:
	CSpotLight() = default;
	~CSpotLight() = default;

	void SetTransform(const CU::Transform& aTransform);
	void SetRotation(const CU::Vector3f& aDirection);
	void SetPosition(const CU::Vector3f& aPosition) override;
	void SetRadius(const float anInnerRadius, const float anOuterRadius);
	void SetShadowCamera(CCamera* aShadowCamera);
	void SetShadowMap(CFullscreenTexture aFullscreenTexture);

public:
	const CU::Transform& GetTransform() const { return myTransform; };
	const CU::Vector3f& GetDirection() const { return myTransform.ToMatrix().GetForward(); };
	const float GetInnerRadius() const { return myInnerRadius; };
	const float GetOuterRadius() const { return myOuterRadius; };
	CCamera* GetShadowCamera() const { return myShadowCamera; };
	CFullscreenTexture& GetShadowMap() { return myShadowMap; };

private:
	CFullscreenTexture myShadowMap;
	CCamera* myShadowCamera;
	CU::Transform myTransform;
	float myInnerRadius = 1.0f;
	float myOuterRadius = 10.0f;


};

