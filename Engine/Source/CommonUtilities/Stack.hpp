#pragma once
#include <vector>
#include <assert.h>

namespace CommonUtilities
{
	template <class T>
	class Stack
	{
	public:
		//Skapar en tom stack
		Stack();

		//Returnerar antal element i stacken
		int GetSize() const;

		//Returnerar det översta elementet i stacken. Kraschar med en assert om
		//stacken är tom.
		const T& GetTop() const;

		//Returnerar det översta elementet i stacken. Kraschar med en assert om
		//stacken är tom.
		T& GetTop();

		//Lägger in ett nytt element överst på stacken
		void Push(const T& aValue);

		//Tar bort det översta elementet från stacken och returnerar det. Kraschar
		//med en assert om stacken är tom.
		T Pop();

	private:
		std::vector<T> myContainer;
		int myTop;
	};

	template<class T>
	inline Stack<T>::Stack()
	{
		myTop = -1;
	}

	template<class T>
	inline int Stack<T>::GetSize() const
	{
		return myTop + 1;
	}

	template<class T>
	inline const T& Stack<T>::GetTop() const
	{
		assert(GetSize() > 0);
		return myContainer[myTop];
	}

	template<class T>
	inline T& Stack<T>::GetTop()
	{
		assert(GetSize() > 0);
		return myContainer[myTop];
	}

	template<class T>
	inline void Stack<T>::Push(const T& aValue)
	{
		myTop++;
		myContainer.push_back(aValue);
	}

	template<class T>
	inline T Stack<T>::Pop()
	{
		assert(GetSize() > 0);
		myTop--;
		T back = myContainer.back();
		myContainer.pop_back();
		return back;
	}
}
namespace CU = CommonUtilities;