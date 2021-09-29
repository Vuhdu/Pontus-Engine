#pragma once
class CLight
{
public:
	CLight() = default;
	~CLight() = default;

	virtual void SetPosition(const CU::Vector3f& aPosition);
	void SetColor(const CU::Vector3f& aColor);
	void SetIntensity(float anIntensity);
	void SetRange(float aRange);

public:
	const CU::Vector3f& GetPosition() const { return myPosition; };
	const CU::Vector3f& GetColor() const { return myColor; };
	const float GetIntensity() const { return myIntensity; };
	const float GetRange() const { return myRange; };

private:
	CU::Matrix4x4f myTransform;

	CU::Vector3f myPosition = { 0.0f, 0.0f, 0.0f };
	CU::Vector3f myColor = { 1.0f, 1.0f, 1.0f };
	float myIntensity = 1.0f;
	float myRange = 10.0f;
};

