#pragma once
#include "Matrix4x4.hpp"

namespace CommonUtilities
{
	class Transform
	{
	public:
		const Vector3f& GetPosition() const;
		void SetPosition(const Vector3f& aPosition);

		const Vector3f& GetRotation() const;
		void SetRotation(const Vector3f& aRotation);

		const Vector3f& GetScale() const;
		void SetScale(const Vector3f& aScale);
		void SetScale(const float aScale);

		Matrix4f ToMatrix() const;

	private:
		Vector3f myPosition;
		Vector3f myRotation;
		Vector3f myScale = { 1.f, 1.f, 1.f };
	};
}
namespace CU = CommonUtilities;