#include "pch.h"
#include "CameraFactory.h"

#include "Camera.h"
#include "Scene.h"

CCameraFactory::CCameraFactory()
{
}

CCameraFactory::~CCameraFactory()
{
}

CCamera* CCameraFactory::CreateCamera(const float aFieldOfView, eOrientation anOrientation)
{
	return CreateCamera(aFieldOfView, 1.0f, 10000.0f, CEngine::GetResolution(), anOrientation);
}

CCamera* CCameraFactory::CreateCamera(const float aFieldOfView, const float aNearPlane, const float aFarPlane, const CU::Vector2ui aResolution, eOrientation anOrientation)
{
	CCamera* camera = new CCamera();
	camera->Init(aFieldOfView, aNearPlane, aFarPlane, aResolution, anOrientation);

	CEngine::GetScene()->AddInstance(camera);

	return camera;
}

CCamera* CCameraFactory::CreateCameraWithoutAddingToScene(const float aFieldOfView, const float aNearPlane, const float aFarPlane, const CU::Vector2ui aResolution, eOrientation anOrientation)
{
	CCamera* camera = new CCamera();
	camera->Init(aFieldOfView, aNearPlane, aFarPlane, aResolution, anOrientation);

	return camera;
}
