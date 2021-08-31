#pragma once

class CCamera;

class CCameraFactory
{
public:
	CCameraFactory();
	~CCameraFactory();

	CCamera* CreateCamera(const float aFieldOfView);

private:

};

