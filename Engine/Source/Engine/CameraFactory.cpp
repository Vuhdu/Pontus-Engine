#include "pch.h"
#include "CameraFactory.h"

#include "Camera.h"

CCameraFactory::CCameraFactory()
{
}

CCameraFactory::~CCameraFactory()
{
}

CCamera* CCameraFactory::CreateCamera(const float aFieldOfView)
{
	CCamera* camera = new CCamera();
	camera->Init(aFieldOfView, { 1920.f, 1080.f });
	return camera;
}
