#pragma once
#include "Light.h"
#include "FullscreenTexture.h"

class CCamera;

class CSpotLight : public CLight
{
public:
	CSpotLight() = default;
	~CSpotLight() = default;

	void SetTransform(const CU::Matrix4x4f& aTransform);
	void SetDirection(const CU::Vector3f& aDirection);
	void SetPosition(const CU::Vector3f& aPosition) override;
	void SetRadius(const float anInnerRadius, const float anOuterRadius);
	void SetShadowCamera(CCamera* aShadowCamera);
	void SetShadowMap(CFullscreenTexture aFullscreenTexture);

public:
	CCamera* GetShadowCamera() const { return myShadowCamera; };
	const CU::Matrix4x4f& GetTransform() const { return myTransform; };
	const CU::Vector3f& GetDirection() const { return myTransform.Forward(); };
	const float GetInnerRadius() const { return myInnerRadius; };
	const float GetOuterRadius() const { return myOuterRadius; };
	CFullscreenTexture& GetShadowMap() { return myShadowMap; };

private:
	CFullscreenTexture myShadowMap;
	CCamera* myShadowCamera;
	CU::Matrix4x4f myTransform;
	float myInnerRadius = 1.0f;
	float myOuterRadius = 10.0f;


};

