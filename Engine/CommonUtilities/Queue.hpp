#pragma once
#include <vector>
#include <assert.h>

namespace CommonUtilities
{
	template <class T>
	class Queue
	{
	public:
		//Skapar en tom k�
		Queue();

		//Returnerar antal element i k�n
		int GetSize() const;

		//Returnerar elementet l�ngst fram i k�n. Kraschar med en assert om k�n �r
		//tom
		const T& GetFront() const;

		//Returnerar elementet l�ngst fram i k�n. Kraschar med en assert om k�n �r
		//tom
		T& GetFront();

		//L�gger in ett nytt element l�ngst bak i k�n
		void Enqueue(const T& aValue);

		//Tar bort elementet l�ngst fram i k�n och returnerar det. Kraschar med en
		//assert om k�n �r tom.
		T Dequeue();

	private:
		std::vector<T> myContainer;
		int myFront;
		int myBack;
		int mySize;
		int myMaxSize = 2;
	};

	template<class T>
	inline Queue<T>::Queue()
	{
		myFront = 0;
		myBack = 0;
		mySize = 0;
		myContainer.reserve(myMaxSize);
		myContainer.resize(myMaxSize);
	}

	template<class T>
	inline int Queue<T>::GetSize() const
	{
		assert(mySize >= 0 && "Queue is empty");
		return mySize;
	}

	template<class T>
	inline const T& Queue<T>::GetFront() const
	{
		assert(mySize > 0 && "Queue is empty");
		return myContainer[myFront];
	}

	template<class T>
	inline T& Queue<T>::GetFront()
	{
		assert(mySize > 0 && "Queue is empty");
		return myContainer[myFront];
	}

	template<class T>
	inline void Queue<T>::Enqueue(const T& aValue)
	{
		mySize++;
		myBack++;

		if ((myFront == 0 && myBack == (myMaxSize - 1)) || myBack == (myFront - 1))
		{
			std::vector<T> newContainer;
			myMaxSize *= 2;
			newContainer.reserve(myMaxSize);
			newContainer.resize(myMaxSize);

			const int oldSize = mySize;
			for (int index = 0; index < oldSize; index++)
			{
				newContainer[index] = Dequeue();
			}

			myContainer = newContainer;
			mySize = oldSize;
			myFront = 0;
			myBack = mySize;
		}
		else if (myBack == myMaxSize - 1)
		{
			myBack = 0;
		}
	
		myContainer[myBack - 1] = aValue;
	}
	
	template<class T>
	inline T Queue<T>::Dequeue()
	{
		assert(mySize > 0);
		mySize--;
		T first = myContainer[myFront];
		myFront++;

		if (myFront > (myMaxSize - 1))
		{
			myFront = 0;
		}

		return first;
	}
}
namespace CU = CommonUtilities;