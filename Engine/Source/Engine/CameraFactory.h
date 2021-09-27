#pragma once

class CCamera;
enum class eOrientation;

class CCameraFactory
{
public:
	CCameraFactory();
	~CCameraFactory();

	CCamera* CreateCamera(const float aFieldOfView, eOrientation anOrientation);

private:

};

