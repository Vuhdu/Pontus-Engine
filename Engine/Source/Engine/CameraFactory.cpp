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
	CCamera* camera = new CCamera();
	camera->Init(aFieldOfView, CEngine::GetResolution(), anOrientation);

	CEngine::GetScene()->AddInstance(camera);

	return camera;
}
