#pragma once
#include <vector>
#include <assert.h>

namespace CommonUtilities
{
	template <class T>
	class MinHeap
	{
	public:
		int GetSize() const; //returnerar antal element i MinHeapen

		void Enqueue(const T& aElement); //lägger till elementet i MinHeapen

		const T& GetTop() const; //returnerar det största elementet i MinHeapen

		T Dequeue(); //tar bort det största elementet ur MinHeapen och returnerar det

	private:
		void BubbleUp(const int aIndex);
		void BubbleDown(const int aIndex);

	private:
		std::vector<T> myMinHeap;

	};

	template<class T>
	inline int MinHeap<T>::GetSize() const
	{
		return myMinHeap.size();
	}

	template<class T>
	inline void MinHeap<T>::Enqueue(const T& aElement)
	{
		myMinHeap.push_back(aElement);
		BubbleUp(myMinHeap.size() - 1);
		return;
	}

	template<class T>
	inline void MinHeap<T>::BubbleUp(const int aIndex)
	{
		if (aIndex <= 0)
		{
			return;
		}

		const int parentIndex = (aIndex - 1) / 2;

		if (myMinHeap[aIndex] < myMinHeap[parentIndex])
		{
			T value = myMinHeap[parentIndex];
			myMinHeap[parentIndex] = myMinHeap[aIndex];
			myMinHeap[aIndex] = value;

			BubbleUp(parentIndex);
		}
	}

	template<class T>
	inline void MinHeap<T>::BubbleDown(const int aIndex)
	{
		const int leftChildIndex = 2 * aIndex + 1;
		const int rightChildIndex = 2 * aIndex + 2;

		if (leftChildIndex > GetSize() - 1 || rightChildIndex > GetSize() - 1)
		{
			return;
		}

		if (myMinHeap[leftChildIndex] < myMinHeap[aIndex])
		{
			T value = myMinHeap[leftChildIndex];
			myMinHeap[leftChildIndex] = myMinHeap[aIndex];
			myMinHeap[aIndex] = value;

			BubbleDown(leftChildIndex);
		}
		if (myMinHeap[rightChildIndex] < myMinHeap[aIndex])
		{
			T value = myMinHeap[rightChildIndex];
			myMinHeap[rightChildIndex] = myMinHeap[aIndex];
			myMinHeap[aIndex] = value;

			BubbleDown(rightChildIndex);
		}
	}

	template<class T>
	inline const T& MinHeap<T>::GetTop() const
	{
		assert(myMinHeap.size() > 0 && "There are no values in this MinHeap.");
		return myMinHeap[0];
	}

	template<class T>
	inline T MinHeap<T>::Dequeue()
	{
		T value = myMinHeap[0];
		myMinHeap[0] = myMinHeap[myMinHeap.size() - 1];
		myMinHeap.pop_back();
		BubbleDown(0);
		return value;
	}
}
namespace CU = CommonUtilities;