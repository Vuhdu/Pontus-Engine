#pragma once
#include "Plane.hpp"
#include "Sphere.hpp"
#include "Ray.hpp"
#include "AABB3D.hpp"

namespace CommonUtilities 
{
	// If the ray is parallel to the plane, aOutIntersectionPoint remains unchanged. If
	// the ray is in the plane, true is returned, if not, false is returned. If the ray
	// isn't parallel to the plane, the intersection point is stored in
	// aOutIntersectionPoint and true returned.
	template<class T>
	bool IntersectionPlaneRay(const Plane<T>& aPlane, const Ray<T>& aRay, Vector3<T>& aOutIntersectionPoint);
	// If the ray intersects the AABB, true is returned, if not, false is returned.
	// A ray in one of the AABB's sides is counted as intersecting it.
	template<class T>
	bool IntersectionAABBRay(const AABB3D<T>& aAABB, const Ray<T>& aRay);
	// If the ray intersects the sphere, true is returned, if not, false is returned.
	// A rat intersecting the surface of the sphere is considered as intersecting it.
	template<class T>
	bool IntersectionSphereRay(const Sphere<T>& aSphere, const Ray<T>& aRay);

	template<class T>
	bool IntersectionPlaneRay(const Plane<T>& aPlane, const Ray<T>& aRay, Vector3<T>& aOutIntersectionPoint)
	{
		return aPlane.IsInside(aRay.myOrigin) == true && (aPlane.GetNormal().Dot(aRay.myDirection) > 0.0f || aPlane.GetNormal().Dot(aRay.myDirection) < 0.0f);
	}

	template<class T>
	bool IntersectionAABBRay(const AABB3D<T>& aAABB, const Ray<T>& aRay)
	{
		if (aAABB.IsInside(aRay.myOrigin))
		{
			return true;
		}
		return false;
	}

	template<class T>
	bool IntersectionSphereRay(const Sphere<T>& aSphere, const Ray<T>& aRay)
	{
		float r = aSphere.myRadius;
		Vector3<T> e = aSphere.myCenter - aRay.myOrigin;
		T dotDir = e.Dot(aRay.myDirection);

		if (dotDir <= 0.0f)
		{
			return false;
		}
		Vector3<T> a = aRay.myDirection * e.Dot(aRay.myDirection);
		return (e.LengthSqr() - a.LengthSqr()) <= (aSphere.myRadius * aSphere.myRadius);
	}
}