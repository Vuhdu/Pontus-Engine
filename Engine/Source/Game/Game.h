#pragma once

struct SCreateParameters;

class CGameWorld;

class CGame 
{
public:
	CGame(SCreateParameters* someCreateParameters);
	~CGame() = default;

	void InitCallback();
	void UpdateCallback();

private:
	CGameWorld* myGameWorld = nullptr;

};