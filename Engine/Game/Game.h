#pragma once

class CGame 
{
public:
	CGame() = default;
	~CGame() = default;

	bool Init();

	void Update();

	void Destroy();

private:
};