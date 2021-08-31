#pragma once
#include <array>
#include <assert.h>
#include <algorithm>
#include <deque>

namespace CommonUtilities
{
	template <class T, unsigned int size>
	class ObjectPool
	{
	public:
		ObjectPool();
		ObjectPool(const std::initializer_list<T>& aInitList);
		~ObjectPool();

		T*			GetObject();				// Returnerar ett ledigt objekt att använda
		void		Recycle(T* aObject);		// Lämnar tillbaka ett använt objekt

	private:
		std::array<T, size> myPool;				// Dom faktiska objekten som används
		std::deque<T*> myFreeObjects;			// Objekt som är lediga att använda
	};

	template<class T, unsigned int size>
	inline ObjectPool<T, size>::ObjectPool()
	{
		assert(myPool.size() > 0 && "Can't create an object pool with size 0 or less.");

		for (int i = 0; i < size; i++)
		{
			myFreeObjects.push_back(&myPool[i]);
		}
	}

	template<class T, unsigned int size>
	inline ObjectPool<T, size>::ObjectPool(const std::initializer_list<T>& aInitList)
	{
		assert(aInitList.size() <= size && "Initializer list too large.");

		int increment = 0;
		for (auto iter = aInitList.begin(); iter != aInitList.end(); ++iter)
		{
			myPool[increment++] = *iter;
		}

		for (int i = 0; i < size; i++)
		{
			myFreeObjects.push_back(&myPool[i]);
		}
	}

	template<class T, unsigned int size>
	inline ObjectPool<T, size>::~ObjectPool()
	{

	}

	template<class T, unsigned int size>
	inline T* ObjectPool<T, size>::GetObject()
	{
		assert(myFreeObjects.size() > 0 && "There are no free objects in the object pool.");

		T* first = myFreeObjects.front();
		myFreeObjects.pop_front();
		return first;
	}

	template<class T, unsigned int size>
	inline void ObjectPool<T, size>::Recycle(T* aObject)
	{
		assert(myFreeObjects.size() < size && "The object pool is full.");

		for (int i = 0; i < size; i++)
		{
			if (myPool[i] == *aObject)
			{
				auto iterator = std::find(myFreeObjects.begin(), myFreeObjects.end(), aObject);
				if (iterator == myFreeObjects.end())
				{
					myFreeObjects.push_back(&myPool[i]);
					return;
				}				
				assert(false && "Attemped to recycle an object already in the pool.");
				return;
			}
		}
		assert(false && "The object does not belong to this object pool.");
	}
}

namespace CU = CommonUtilities;

