#pragma once

struct SCreateParameters;

class CModelInstance;
class CSpotLight;

class CGame 
{
public:
	CGame(SCreateParameters* someCreateParameters);
	~CGame() = default;

	void InitCallback();
	void UpdateCallback();

private:
	CModelInstance* myHead = nullptr;
	CModelInstance* myHead2 = nullptr;

	CSpotLight* mySpotLight = nullptr;

};