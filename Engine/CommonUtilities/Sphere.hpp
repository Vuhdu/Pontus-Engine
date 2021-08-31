#pragma once
#include "Vector3.hpp"

namespace CommonUtilities
{
	template<class T>
	class Sphere
	{
	public:
		// Default constructor: there is no sphere, the radius is zero and the position is
		// the zero vector.
		Sphere() = default;
		// Copy constructor.
		Sphere(const Sphere<T>& aSphere);
		// Constructor that takes the center position and radius of the sphere.
		Sphere(const Vector3<T>& aCenter, T aRadius);

		// Init the sphere with a center and a radius, the same as the constructor above.
		void InitWithCenterAndRadius(const Vector3<T>& aCenter, T aRadius);

		// Returns whether a point is inside the sphere: it is inside when the point is on the
		// sphere surface or inside of the sphere.
		bool IsInside(const Vector3<T>& aPosition) const;

		Vector3<T> myCenter;
		T myRadius = 0;
	private:
	};

	template<class T>
	inline Sphere<T>::Sphere(const Sphere<T>& aSphere)
	{
		InitWithCenterAndRadius(aSphere.myCenter, aSphere.myRadius);
	}

	template<class T>
	inline Sphere<T>::Sphere(const Vector3<T>& aCenter, T aRadius)
	{
		InitWithCenterAndRadius(aCenter, aRadius);
	}

	template<class T>
	inline void Sphere<T>::InitWithCenterAndRadius(const Vector3<T>& aCenter, T aRadius)
	{
		myCenter = aCenter;
		myRadius = aRadius;
	}

	template<class T>
	inline bool Sphere<T>::IsInside(const Vector3<T>& aPosition) const
	{
		Vector3<T> distance = aPosition - myCenter;
		return distance.LengthSqr() <= (myRadius * myRadius);
	}
}
namespace CU = CommonUtilities;