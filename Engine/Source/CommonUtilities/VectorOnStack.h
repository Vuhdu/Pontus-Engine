#pragma once
#include <array>
#include <algorithm>
#include <assert.h>

namespace CommonUtilities
{
	template <typename Type, int size, typename CountType = unsigned short, bool UseSafeModeFlag = true>

	class VectorOnStack
	{
	public:
		VectorOnStack();
		VectorOnStack(const VectorOnStack& aVectorOnStack);
		VectorOnStack(const std::initializer_list<Type>& aInitList);
		~VectorOnStack();

		Type* begin();
		const Type* begin() const;
		Type* end();
		const Type* end() const;

		VectorOnStack& operator=	(const VectorOnStack& aVectorOnStack);
		inline const Type& operator[]	(const CountType aIndex) const;
		inline Type& operator[]	(const CountType aIndex);

		inline void Add(const Type& aObject);
		inline void Insert(const CountType aIndex, const Type& aObject);
		inline void RemoveCyclic(const Type& aObject);
		inline void RemoveCyclicAtIndex(const CountType aIndex);
		inline void Clear();
		inline CountType Size() const;

	private:
		std::array<Type, size> myArray;
		CountType myCurrentSize;
	};

	template<typename Type, int size, typename CountType, bool UseSafeModeFlag>
	inline VectorOnStack<Type, size, CountType, UseSafeModeFlag>::VectorOnStack()
	{
		myCurrentSize = 0;
	}

	template<typename Type, int size, typename CountType, bool UseSafeModeFlag>
	VectorOnStack<Type, size, CountType, UseSafeModeFlag>::VectorOnStack(const VectorOnStack& aVectorOnStack) : myCurrentSize(0)
	{
		operator=(aVectorOnStack);
	}

	template<typename Type, int size, typename CountType, bool UseSafeModeFlag>
	inline VectorOnStack<Type, size, CountType, UseSafeModeFlag>::VectorOnStack(const std::initializer_list<Type>& aInitList) : myCurrentSize(0)
	{
		assert(aInitList.size() <= size && "Initializer list too large.");

		for (auto iter = aInitList.begin(); iter != aInitList.end(); ++iter)
		{
			myArray[myCurrentSize++] = *iter;
		}
	}

	template<typename Type, int size, typename CountType, bool UseSafeModeFlag>
	inline VectorOnStack<Type, size, CountType, UseSafeModeFlag>::~VectorOnStack()
	{
		myCurrentSize = 0;
	}

	template<typename Type, int size, typename CountType, bool UseSafeModeFlag>
	inline Type* VectorOnStack<Type, size, CountType, UseSafeModeFlag>::begin()
	{
		return &myArray[0];
	}

	template<typename Type, int size, typename CountType, bool UseSafeModeFlag>
	inline const Type* VectorOnStack<Type, size, CountType, UseSafeModeFlag>::begin() const
	{
		return &myArray[0];
	}

	template<typename Type, int size, typename CountType, bool UseSafeModeFlag>
	inline Type* VectorOnStack<Type, size, CountType, UseSafeModeFlag>::end()
	{
		return &myArray[0] + myCurrentSize;
	}

	template<typename Type, int size, typename CountType, bool UseSafeModeFlag>
	inline const Type* VectorOnStack<Type, size, CountType, UseSafeModeFlag>::end() const
	{
		return &myArray[0] + myCurrentSize;
	}

	template<typename Type, int size, typename CountType, bool UseSafeModeFlag>
	VectorOnStack<Type, size, CountType, UseSafeModeFlag>& VectorOnStack<Type, size, CountType, UseSafeModeFlag>::operator=(const VectorOnStack& aVectorOnStack)
	{
		if (UseSafeModeFlag)
		{
			myArray = aVectorOnStack.myArray;
			myCurrentSize = aVectorOnStack.myCurrentSize;
		}
		else
		{
			memcpy(this, &aVectorOnStack, sizeof(VectorOnStack));
		}
		return *this;
	}

	template<typename Type, int size, typename CountType, bool UseSafeModeFlag>
	inline const Type& VectorOnStack<Type, size, CountType, UseSafeModeFlag>::operator[](const CountType aIndex) const
	{
		assert(myCurrentSize > 0 && "Array is empty.");
		assert((aIndex >= 0 && aIndex <= myCurrentSize) && "Failed to access value. Out of bounds");
		return myArray.at(aIndex);
	}

	template<typename Type, int size, typename CountType, bool UseSafeModeFlag>
	inline Type& VectorOnStack<Type, size, CountType, UseSafeModeFlag>::operator[](const CountType aIndex)
	{
		assert(myCurrentSize > 0 && "Array is empty.");
		assert((aIndex >= 0 && aIndex <= myCurrentSize) && "Failed to access value. Out of bounds");
		return myArray.at(aIndex);
	}

	template<typename Type, int size, typename CountType, bool UseSafeModeFlag>
	inline void VectorOnStack<Type, size, CountType, UseSafeModeFlag>::Add(const Type& aObject)
	{
		assert(myCurrentSize < size && "Can't do add. VectorOnStack is already full.");
		if (UseSafeModeFlag)
		{
			myArray[myCurrentSize] = aObject;
		}
		else
		{
			memcpy(&myArray[myCurrentSize], &aObject, sizeof(Type));
		}
		myCurrentSize++;
	}

	template<typename Type, int size, typename CountType, bool UseSafeModeFlag>
	inline void VectorOnStack<Type, size, CountType, UseSafeModeFlag>::Insert(const CountType aIndex, const Type& aObject)
	{
		assert((aIndex >= 0 && aIndex <= myCurrentSize) && "Can't insert out of bounds.");
		assert(myCurrentSize < size && "Can't do insert. VectorOnStack is already full.");

		if (myCurrentSize == 0)
		{
			Add(aObject);
			return;
		}

		for (CountType index = myCurrentSize; index > aIndex; --index)
		{
			if (UseSafeModeFlag)
			{
				myArray[index] = myArray[index - 1];
			}
			else
			{
				memcpy(&myArray[index], &myArray[index - 1], sizeof(Type));
			}
		}

		if (UseSafeModeFlag)
		{
			myArray[aIndex] = aObject;
		}
		else
		{
			memcpy(&myArray[aIndex], &aObject, sizeof(Type));
		}
		myCurrentSize++;
	}

	template<typename Type, int size, typename CountType, bool UseSafeModeFlag>
	inline void VectorOnStack<Type, size, CountType, UseSafeModeFlag>::RemoveCyclic(const Type& aObject)
	{
		for (CountType index = 0; index < myCurrentSize; index++)
		{
			if (myArray.at(index) == aObject)
			{
				if (index == myCurrentSize)
				{
					myCurrentSize--;
					return;
				}
				myArray.at(index) = myArray.at(myCurrentSize - 1);
				myCurrentSize--;
				return;
			}
		}
		assert(false && "aObject could not be found inside VectorOnStack.");
	}

	template<typename Type, int size, typename CountType, bool UseSafeModeFlag>
	inline void VectorOnStack<Type, size, CountType, UseSafeModeFlag>::RemoveCyclicAtIndex(const CountType aIndex)
	{
		assert((aIndex >= 0 && aIndex < myCurrentSize) && "Out of bounds.");
		myArray.at(aIndex) = myArray.at(myCurrentSize - 1);
		myCurrentSize--;
	}

	template<typename Type, int size, typename CountType, bool UseSafeModeFlag>
	inline void VectorOnStack<Type, size, CountType, UseSafeModeFlag>::Clear()
	{
		myCurrentSize = 0;
	}

	template<typename Type, int size, typename CountType, bool UseSafeModeFlag>
	inline CountType VectorOnStack<Type, size, CountType, UseSafeModeFlag>::Size() const
	{
		return myCurrentSize;
	}
}