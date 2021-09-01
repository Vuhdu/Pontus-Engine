#pragma once
#include <vector>
#include <assert.h>
#include <algorithm>

namespace CommonUtilities
{
	template <class T>
	void SelectionSort(std::vector<T>& aVector);

	template <class T>
	void BubbleSort(std::vector<T>& aVector);

	template <class T>
	void QuickSort(std::vector<T>& aVector);

	template <class T>
	void MergeSort(std::vector<T>& aVector);


	namespace Sort
	{
		template <class T>
		void QuickSort(std::vector<T>& aVector, const int aLow, const int aHigh);

		template <class T>
		int Partition(std::vector<T>& aVector, const int aLow, const int aHigh);

		template <class T>
		void Swap(T* aFirstElement, T* aSecondElement);

		template <class T>
		void Merge(std::vector<T>& aVector, const int aLowIndex, const int aHighIndex, const int aMidIndex);

		template <class T>
		void MergeSort(std::vector<T>& aVector, const int aLowIndex, const int aHighIndex);
	}


	template<class T>
	inline void SelectionSort(std::vector<T>& aVector)
	{
		int size = aVector.size();

		assert(size > 1 && "Can't selection sort. Vector is too small.");

		int smallestElement = 0;
		for (int i = 0; i < size - 1; i++)
		{			
			smallestElement = i;
			for (int j = i + 1; j < size; j++)
			{
				if (aVector[j] < aVector[smallestElement])
				{
					smallestElement = j;
				}							
			}
			Sort::Swap(&aVector[smallestElement], &aVector[i]);				
		}
	}
	
	template<class T>
	inline void BubbleSort(std::vector<T>& aVector)
	{
		int size = aVector.size();

		assert(size > 1 && "Can't bubble sort. Vector is too small.");

		for (int i = 0; i < size - 1; i++)
		{
			for (int j = size - 1; j > i; --j)
			{
				if (aVector[j] < aVector[j - 1])
				{
					Sort::Swap(&aVector[j], &aVector[j - 1]);
				}				 
			}				 
		}
	}

	template<class T>
	inline void QuickSort(std::vector<T>& aVector)
	{
		assert(aVector.size() > 1 && "Can't quick sort. Vector is too small.");
		Sort::QuickSort(aVector, 0, aVector.size() - 1);
	}

	template<class T>
	inline void MergeSort(std::vector<T>& aVector)
	{
		assert(aVector.size() > 1 && "Can't merge sort. Vector is too small.");
		Sort::MergeSort(aVector, 0, aVector.size() - 1);
	}

	template<class T>
	inline void Sort::MergeSort(std::vector<T>& aVector, const int aLowIndex, const int aHighIndex)
	{
		if(aLowIndex < aHighIndex)
		{
			int midIndex = (aLowIndex + aHighIndex) / 2;
			Sort::MergeSort(aVector, aLowIndex, midIndex);
			Sort::MergeSort(aVector, midIndex + 1, aHighIndex);
			Sort::Merge(aVector, aLowIndex, aHighIndex, midIndex);
		}
	}

	template<class T>
	inline void Sort::Merge(std::vector<T>& aVector, const int aLowIndex, const int aHighIndex, const int aMidIndex)
	{
		int tempVectorIndex = aLowIndex;
		int leftVectorIndex = aLowIndex;
		int rightVectorIndex = aMidIndex + 1;
		
		std::vector<T> tempVector;
		tempVector.resize(aVector.size());

		while(leftVectorIndex <= aMidIndex && rightVectorIndex <= aHighIndex)
		{
			if(aVector[leftVectorIndex] < aVector[rightVectorIndex])
			{
				tempVector[tempVectorIndex] = aVector[leftVectorIndex];
				tempVectorIndex++;
				leftVectorIndex++;
			}
			else
			{
				tempVector[tempVectorIndex] = aVector[rightVectorIndex];
				tempVectorIndex++;
				rightVectorIndex++;
			}
		}

		while (leftVectorIndex <= aMidIndex) 
		{
			tempVector[tempVectorIndex] = aVector[leftVectorIndex];
			tempVectorIndex++;
			leftVectorIndex++;
		}
		while (rightVectorIndex <= aHighIndex) 
		{
			tempVector[tempVectorIndex] = aVector[rightVectorIndex];
			tempVectorIndex++;
			rightVectorIndex++;
		}

		for(int i = aLowIndex; i < tempVectorIndex; i++)
		{
			aVector[i] = tempVector[i];
		}
	}

	template<class T>
	inline void Sort::QuickSort(std::vector<T>& aVector, const int aLow, const int aHigh)
	{
		if (aLow < aHigh)
		{
			const int pi = Sort::Partition(aVector, aLow, aHigh);

			Sort::QuickSort(aVector, aLow, pi - 1);
			Sort::QuickSort(aVector, pi + 1, aHigh);
		}
	}

	template<class T>
	inline int Sort::Partition(std::vector<T>& aVector, const int aLow, const int aHigh)
	{
		T pivot = aVector[aHigh];
		int index = (aLow - 1);

		for (int j = aLow; j <= aHigh - 1; j++)
		{
			if (aVector[j] < pivot || (aVector[j] < pivot && pivot < aVector[j]))
			{
				index++;
				Sort::Swap(&aVector[index], &aVector[j]);
			}
		}
		Sort::Swap(&aVector[index + 1], &aVector[aHigh]);
		return (index + 1);
	}

	template<class T>
	inline void Sort::Swap(T* aFirstElement, T* aSecondElement)
	{
		T temp = *aFirstElement;
		*aFirstElement = *aSecondElement;
		*aSecondElement = temp;
	}
}

namespace CU = CommonUtilities;