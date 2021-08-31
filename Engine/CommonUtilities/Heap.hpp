#pragma once
#include <vector>
#include <assert.h>

namespace CommonUtilities
{
	template <class T>
	class Heap
	{
	public:
		int GetSize() const; //returnerar antal element i heapen

		void Enqueue(const T& aElement); //lägger till elementet i heapen

		const T& GetTop() const; //returnerar det största elementet i heapen

		T Dequeue(); //tar bort det största elementet ur heapen och returnerar det
	
	private:
		void BubbleUp(const int aIndex);
		void BubbleDown(const int aIndex);


	private:
		std::vector<T> myHeap;

	};

	template<class T>
	inline int Heap<T>::GetSize() const
	{
		return myHeap.size();
	}

	template<class T>
	inline void Heap<T>::Enqueue(const T& aElement)
	{
		myHeap.push_back(aElement);
		BubbleUp(myHeap.size() - 1);
		return;
	}

	template<class T>
	inline void Heap<T>::BubbleUp(const int aIndex)
	{
		if(aIndex <= 0)
		{
			return;
		}

		const int parentIndex = (aIndex - 1) / 2;

		if (myHeap[parentIndex] < myHeap[aIndex])
		{
			T value = myHeap[parentIndex];
			myHeap[parentIndex] = myHeap[aIndex];
			myHeap[aIndex] = value;

			BubbleUp(parentIndex);
		}		
	}

	template<class T>
	inline void Heap<T>::BubbleDown(const int aIndex)
	{
		const int leftChildIndex = 2 * aIndex + 1;
		const int rightChildIndex = 2 * aIndex + 2;
		
		if (leftChildIndex > GetSize() - 1 || rightChildIndex > GetSize() - 1)
		{
			return;
		}

		if (myHeap[aIndex] < myHeap[leftChildIndex])
		{
			T value = myHeap[leftChildIndex];
			myHeap[leftChildIndex] = myHeap[aIndex];
			myHeap[aIndex] = value;

			BubbleDown(leftChildIndex);
		}
		if (myHeap[aIndex] < myHeap[rightChildIndex])
		{
			T value = myHeap[rightChildIndex];
			myHeap[rightChildIndex] = myHeap[aIndex];
			myHeap[aIndex] = value;

			BubbleDown(rightChildIndex);
		}
	}

	template<class T>
	inline const T& Heap<T>::GetTop() const
	{
		assert(myHeap.size() > 0 && "There are no values in this heap.");
		return myHeap[0];
	}

	template<class T>
	inline T Heap<T>::Dequeue()
	{
		T value = myHeap[0];
		myHeap[0] = myHeap[myHeap.size() - 1];
		myHeap.pop_back();
		BubbleDown(0);
		return value;
	}
}
namespace CU = CommonUtilities;