#include "pch.h"
#include "PointLight.h"

#include "Camera.h"

void CPointLight::SetPosition(const CU::Vector3f& aPosition)
{
	CLight::SetPosition(aPosition);
	if (myShouldRenderShadows)
	{
		for (int i = 0; i < 6; i++)
		{
			if (myShadowCameras[i])
			{
				myShadowCameras[i]->SetPosition(aPosition);
			}
		}
	}
}

void CPointLight::SetShouldRenderShadows(const bool aShouldRenderShadows)
{
	myShouldRenderShadows = aShouldRenderShadows;
}

void CPointLight::SetShadowCameras(CCamera* aShadowCamera, int anIndex)
{
	myShadowCameras[anIndex] = aShadowCamera;
}

void CPointLight::SetShadowMaps(CFullscreenTexture aFullscreenTexture, int anIndex)
{
	myShadowMaps[anIndex] = aFullscreenTexture;
}
