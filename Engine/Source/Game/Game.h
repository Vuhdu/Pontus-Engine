#pragma once

struct SCreateParameters;

class CGame 
{
public:
	CGame(SCreateParameters* someCreateParameters);
	~CGame() = default;

	void InitCallback();

	void UpdateCallback();

private:
	bool myUpdateHasPassed = false;

};