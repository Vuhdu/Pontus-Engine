#pragma once

namespace CommonUtilities
{
	template <class T>
	class DoublyLinkedList;

	template <class T>
	class DoublyLinkedListNode
	{
	public:
		// Copy-konstruktorn och assignment-operatorn �r borttagna, s� att det enda
		// s�ttet att skapa en nod �r genom att stoppa in ett v�rde i en lista.
		DoublyLinkedListNode<T>(const DoublyLinkedListNode<T>&) = delete;

		DoublyLinkedListNode<T>& operator=(const DoublyLinkedListNode<T>&) = delete;

		// Returnerar nodens v�rde
		const T& GetValue() const;
		T& GetValue();

		// Returnerar n�sta nod i listan, eller nullptr om noden �r sist i listan
		DoublyLinkedListNode<T>* GetNext() const;

		// Returnerar f�reg�ende nod i listan, eller nullptr om noden �r f�rst i
		// listan
		DoublyLinkedListNode<T>* GetPrevious() const;
	private:
		// Konstruktorn och destruktorn �r privat, s� att man inte kan skapa eller
		// ta bort noder utifr�n. List-klassen �r friend, s� att den kan skapa
		// eller ta bort noder.
		friend class DoublyLinkedList<T>;
		DoublyLinkedListNode(const T& aValue);
		~DoublyLinkedListNode() 
		{
			myNext = nullptr;
			myPrevious = nullptr;
		}

		T myValue;
		DoublyLinkedListNode<T>* myNext = nullptr;
		DoublyLinkedListNode<T>* myPrevious = nullptr;

	};

	template<class T>
	inline DoublyLinkedListNode<T>::DoublyLinkedListNode(const T& aValue)
	{
		myValue = aValue;
		myNext = nullptr;
		myPrevious = nullptr;
	}

	template<class T>
	inline const T& DoublyLinkedListNode<T>::GetValue() const
	{
		return myValue;
	}

	template<class T>
	inline T& DoublyLinkedListNode<T>::GetValue()
	{
		return myValue;
	}

	template<class T>
	inline DoublyLinkedListNode<T>* DoublyLinkedListNode<T>::GetNext() const
	{
		return myNext;
	}

	template<class T>
	inline DoublyLinkedListNode<T>* DoublyLinkedListNode<T>::GetPrevious() const
	{
		return myPrevious;
	}
}
namespace CU = CommonUtilities;