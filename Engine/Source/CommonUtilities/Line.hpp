#pragma once
#include "Vector2.hpp"

namespace CommonUtilities 
{
	template <class T>
	class Line
	{
	public:
		// Default constructor: there is no line, the normal is the zero vector.
		Line() = default;

		// Copy constructor.
		Line(const Line <T>& aLine);

		// Constructor that takes two points that define the line, the direction is aPoint1 - aPoint0.
		Line(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1);

		// Init the line with two points, the same as the constructor above.
		void InitWith2Points(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1);

		// Init the line with a point and a direction.
		void InitWithPointAndDirection(const Vector2<T>& aPoint, const Vector2<T>& aDirection);
		
		// Returns whether a point is inside the line: it is inside when the point is on the line or on the side the normal is pointing away from.
		bool IsInside(const Vector2<T> & aPosition) const;
	
		// Returns the direction of the line.
		const Vector2<T>& GetDirection() const;
	
		// Returns the normal of the line, which is (-direction.y, direction.x).
		const Vector2<T> GetNormal() const;
	
	private:
		Vector2<T> myPoint;
		Vector2<T> myDirection;
	};

	template<class T>
	inline Line<T>::Line(const Line<T>& aLine)
	{
		myPoint = aLine.myPoint;
		myDirection = aLine.myDirection;
	}

	template<class T>
	inline Line<T>::Line(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1)
	{
		InitWith2Points(aPoint0, aPoint1);
	}

	//template <class T>
	//inline bool operator==(const Line<T>& aLine0, const Line<T>& aLine1)
	//{
	//	if (aLine0.myDirection.x == aLine1.myDirection.x &&
	//		aLine0.myDirection.y == aLine1.myDirection.y &&
	//		aLine0.myPoint.x == aLine1.myPoint.x &&
	//		aLine0.myPoint.y == aLine1.myPoint.y)
	//	{
	//		return true;
	//	}
	//	return false;
	//}

	template<class T>
	inline void Line<T>::InitWith2Points(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1)
	{
		myPoint = aPoint0;
		myDirection = Vector2<T>(aPoint1 - aPoint0).GetNormalized();
	}

	template<class T>
	inline void Line<T>::InitWithPointAndDirection(const Vector2<T>& aPoint, const Vector2<T>& aDirection)
	{
		myPoint = aPoint;
		myDirection = aDirection.GetNormalized();
	}

	template<class T>
	inline bool Line<T>::IsInside(const Vector2<T>& aPosition) const
	{
		Vector2<T> dir = aPosition - myPoint;
		return dir.Dot(GetNormal()) <= 0;
	}

	template<class T>
	inline const Vector2<T>& Line<T>::GetDirection() const
	{
		return myDirection;
	}

	template<class T>
	inline const Vector2<T> Line<T>::GetNormal() const
	{
		return Vector2<T>(-myDirection.y, myDirection.x);
	}
}

namespace CU = CommonUtilities;