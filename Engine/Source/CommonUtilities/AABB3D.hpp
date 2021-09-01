#pragma once
#include "Vector3.hpp"

namespace CommonUtilities
{
	template<class T>
	class AABB3D
	{
	public:
		// Default constructor: there is no AABB, both min and max points are the zero vector.
		AABB3D() = default;

		// Copy constructor.
		AABB3D(const AABB3D<T>& aAABB3D);

		// Constructor taking the positions of the minimum and maximum corners.
		AABB3D(const Vector3<T>& aMin, const Vector3<T>& aMax);

		// Init the AABB with the positions of the minimum and maximum corners, same as
		// the constructor above.
		void InitWithMinAndMax(const Vector3<T>& aMin, const Vector3<T>& aMax);

		// Returns whether a point is inside the AABB: it is inside when the point is on any
		// of the AABB's sides or inside of the AABB.
		bool IsInside(const Vector3<T>& aPosition) const;

		Vector3<T> myMin;
		Vector3<T> myMax;
	private:
	};

	template<class T>
	inline AABB3D<T>::AABB3D(const AABB3D<T>& aAABB3D)
	{
		InitWithMinAndMax(aAABB3D.myMin, aAABB3D.myMax);
	}

	template<class T>
	inline AABB3D<T>::AABB3D(const Vector3<T>& aMin, const Vector3<T>& aMax)
	{
		InitWithMinAndMax(aMin, aMax);
	}

	template<class T>
	inline void AABB3D<T>::InitWithMinAndMax(const Vector3<T>& aMin, const Vector3<T>& aMax)
	{
		myMin = aMin;
		myMax = aMax;
	}

	template<class T>
	inline bool AABB3D<T>::IsInside(const Vector3<T>& aPosition) const
	{
		if (myMax.x >= aPosition.x && myMax.y >= aPosition.y && myMax.z >= aPosition.z)
		{
			if (myMin.x <= aPosition.x && myMin.y <= aPosition.y && myMin.z <= aPosition.z)
			{
				return true;
			}
		}
		return false;
	}
}
namespace CU = CommonUtilities;