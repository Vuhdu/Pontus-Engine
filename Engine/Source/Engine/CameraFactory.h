#pragma once

class CCamera;
enum class eOrientation;

class CCameraFactory
{
public:
	CCameraFactory();
	~CCameraFactory();

	CCamera* CreateCamera(const float aFieldOfView, eOrientation anOrientation);
	CCamera* CreateCamera(const float aFieldOfView, const CU::Vector2ui aResolution, eOrientation anOrientation);
	CCamera* CreateCameraWithoutAddingToScene(const float aFieldOfView, const CU::Vector2ui aResolution, eOrientation anOrientation);

private:

};

