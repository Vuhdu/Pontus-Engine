#pragma once
#include "Vector3.hpp"
#include "Sphere.hpp"

namespace CommonUtilities
{
	template <class T>
	class Plane
	{
	public:
		// Default constructor.
		Plane() = default;

		// Constructor taking three points where the normal is (aPoint1 - aPoint0) x (aPoint2 -aPoint0).
		Plane(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2);
		
		// Constructor taking a point and a normal.
		Plane(const Vector3<T>& aPoint0, const Vector3<T>& aNormal);
		
		// Init the plane with three points, the same as the constructor above.
		void InitWith3Points(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2);
		
		// Init the plane with a point and a normal, the same as the constructor above.
		void InitWithPointAndNormal(const Vector3<T>& aPoint, const Vector3<T>& aNormal);
		
		// Returns whether a point is inside the plane: it is inside when the point is on the plane or on the side the normal is pointing away from.
		bool IsInside(const Vector3<T> & aPosition) const;

		// Returns whether a sphere is inside the plane.
		bool IsInside(const Sphere<T> & aSphere) const;
		
		// Returns the normal of the plane.
		const Vector3<T>& GetNormal() const;

		Vector3<T> myPoint;
		Vector3<T> myNormal;
	private:
	};

	template<class T>
	inline Plane<T>::Plane(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2)
	{
		InitWith3Points(aPoint0, aPoint1, aPoint2);
	}

	template<class T>
	inline Plane<T>::Plane(const Vector3<T>& aPoint0, const Vector3<T>& aNormal)
	{
		InitWithPointAndNormal(aPoint0, aNormal);
	}

	template<class T>
	inline void Plane<T>::InitWith3Points(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2)
	{
		myPoint = aPoint0;
		Vector3<T> dirA = aPoint1 - aPoint0;
		Vector3<T> dirB = aPoint2 - aPoint0;
		myNormal = dirA.Cross(dirB).GetNormalized();
	}

	template<class T>
	inline void Plane<T>::InitWithPointAndNormal(const Vector3<T>& aPoint, const Vector3<T>& aNormal)
	{
		myPoint = aPoint;
		myNormal = aNormal.GetNormalized();
	}

	template<class T>
	inline bool Plane<T>::IsInside(const Vector3<T>& aPosition) const
	{
		Vector3<T> dir = aPosition - myPoint;
		return dir.Dot(GetNormal()) <= 0;
	}

	template<class T>
	inline bool Plane<T>::IsInside(const Sphere<T>& aSphere) const
	{
		Vector3<float> vector = aSphere.myCenter - myPoint;
		float dotProduct = vector.Dot(myNormal);
		if (dotProduct <= 0)
		{
			return true;
		}
		vector = dotProduct * myNormal;
		return vector.Length() <= aSphere.myRadius;
	}

	template<class T>
	inline const Vector3<T>& Plane<T>::GetNormal() const
	{
		return myNormal;
	}
}
namespace CU = CommonUtilities;