#pragma once
#include "Plane.hpp"
#include "Sphere.hpp"
#include <vector>

namespace CommonUtilities
{
	template <class T>
	class PlaneVolume
	{
	public:
		// Default constructor: empty PlaneVolume.
		PlaneVolume() = default;

		// Constructor taking a list of Plane that makes up the PlaneVolume.
		PlaneVolume(const std::vector<Plane<T>>& aPlaneList);

		// Add a Plane to the PlaneVolume.
		void AddPlane(const Plane<T>& aPlane);
		
		// Returns whether a point is inside the PlaneVolume: it is inside when the point is on the
		// plane or on the side the normal is pointing away from for all the planes in the PlaneVolume.
		bool IsInside(const Vector3<T>& aPosition);

		bool IsInside(const Sphere<T>& aSphere);

	private:
		std::vector<Plane<T>> myPlanes;
	};


	template<class T>
	inline PlaneVolume<T>::PlaneVolume(const std::vector<Plane<T>>& aPlaneList)
	{
		myPlanes = aPlaneList;
	}

	template<class T>
	inline void PlaneVolume<T>::AddPlane(const Plane<T>& aPlane)
	{
		myPlanes.push_back(aPlane);
	}

	template<class T>
	inline bool PlaneVolume<T>::IsInside(const Vector3<T>& aPosition)
	{
		for (unsigned int planeIndex = 0; planeIndex < myPlanes.size(); planeIndex++)
		{
			if (!myPlanes[planeIndex].IsInside(aPosition))
			{
				return false;
			}
		}
		return true;
	}

	template<class T>
	inline bool PlaneVolume<T>::IsInside(const Sphere<T>& aSphere)
	{
		for (unsigned int planeIndex = 0; planeIndex < myPlanes.size(); planeIndex++)
		{
			if (myPlanes[planeIndex].IsInside(aSphere) == false)
			{
				return false;
			}
		}
		return true;
	}
}
namespace CU = CommonUtilities;