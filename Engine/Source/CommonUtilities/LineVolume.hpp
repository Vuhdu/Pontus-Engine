#pragma once
#include "Line.hpp"
#include <vector>

namespace CommonUtilities
{
	template <class T>
	class LineVolume
	{
	public:
		// Default constructor: empty LineVolume.
		LineVolume() = default;

		// Constructor taking a list of Line that makes up the LineVolume.
		LineVolume(const std::vector<Line<T>>& aLineList);

		// Add a Line to the LineVolume.
		void AddLine(const Line<T>& aLine);

		// Returns whether a point is inside the LineVolume: it is inside when the point is
		// on the line or on the side the normal is pointing away from for all the lines in
		// the LineVolume.
		bool IsInside(const Vector2<T>& aPosition);

	private:
		std::vector<Line<T>> myLines;
	};

	template<class T>
	inline LineVolume<T>::LineVolume(const std::vector<Line<T>>& aLineList)
	{
		myLines = aLineList;
	}

	template<class T>
	inline void LineVolume<T>::AddLine(const Line<T>& aLine)
	{
		myLines.push_back(aLine);
	}

	template<class T>
	inline bool LineVolume<T>::IsInside(const Vector2<T>& aPosition)
	{
		for (unsigned int lineIndex = 0; lineIndex < myLines.size(); lineIndex++)
		{
			if (!myLines[lineIndex].IsInside(aPosition))
			{
				return false;
			}
		}
		return true;
	}
}

namespace CU = CommonUtilities;