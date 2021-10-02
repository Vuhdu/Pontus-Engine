#pragma once
#include "Light.h"

#include "FullscreenTexture.h"

class CCamera;

class CPointLight : public CLight
{
public:
	CPointLight() = default;
	~CPointLight() = default;

	void SetPosition(const CU::Vector3f& aPosition) override;

	void SetShouldRenderShadows(const bool aShouldRenderShadows);
	void SetShadowCameras(CCamera* aShadowCamera, int anIndex);
	void SetShadowMaps(CFullscreenTexture aFullscreenTexture, int anIndex);

public:
	const bool IsRenderingShadows() const { return myShouldRenderShadows; };
	std::array<CCamera*, 6>& GetShadowCameras() { return myShadowCameras; };
	std::array<CFullscreenTexture, 6>& GetShadowMaps() { return myShadowMaps; };

private:
	bool myShouldRenderShadows = false;
	std::array<CCamera*, 6> myShadowCameras;
	std::array<CFullscreenTexture, 6> myShadowMaps;

};

