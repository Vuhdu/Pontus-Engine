#pragma once
#include "Vector3.hpp"

namespace CommonUtilities
{
	template<class T>
	class Ray
	{
	public:
		// Default constructor: there is no ray, the origin and direction are the
		// zero vector.
		Ray() = default;
		// Copy constructor.
		Ray(const Ray<T>& aRay);
		// Constructor that takes two points that define the ray, the direction is
		// aPoint - aOrigin and the origin is aOrigin.
		Ray(const Vector3<T>& aOrigin, const Vector3<T>& aPoint);
		// Init the ray with two points, the same as the constructor above.
		void InitWith2Points(const Vector3<T>& aOrigin, const Vector3<T>& aPoint);
		// Init the ray with an origin and a direction.
		void InitWithOriginAndDirection(const Vector3<T>& aOrigin, const Vector3<T>& aDirection);
		Vector3<T> myOrigin;
		Vector3<T> myDirection;
	private:
	};

	template<class T>
	inline Ray<T>::Ray(const Ray<T>& aRay)
	{
		InitWithOriginAndDirection(aRay.myOrigin, aRay.myDirection);
	}

	template<class T>
	inline Ray<T>::Ray(const Vector3<T>& aOrigin, const Vector3<T>& aPoint)
	{
		Vector3<T> direction = aPoint - aOrigin;
		InitWithOriginAndDirection(aOrigin, direction);
	}

	template<class T>
	inline void Ray<T>::InitWith2Points(const Vector3<T>& aOrigin, const Vector3<T>& aPoint)
	{
		Vector3<T> direction = aPoint - aOrigin;
		InitWithOriginAndDirection(aRay.myOrigin, direction);
	}

	template<class T>
	inline void Ray<T>::InitWithOriginAndDirection(const Vector3<T>& aOrigin, const Vector3<T>& aDirection)
	{
		myOrigin = aOrigin;
		myDirection = aDirection.GetNormalized();
	}
}
namespace CU = CommonUtilities;

// destination - origin