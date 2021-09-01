#pragma once
#include "DoublyLinkedNode.hpp"
#include <assert.h>

namespace CommonUtilities
{
	template <class T>
	class DoublyLinkedList
	{
	public:
		// Skapar en tom lista
		DoublyLinkedList();

		// Frigör allt minne som listan allokerat
		~DoublyLinkedList();

		// Returnerar antalet element i listan
		int GetSize() const;

		// Returnerar första noden i listan, eller nullptr om listan är tom
		DoublyLinkedListNode<T>* GetFirst();

		// Returnerar sista noden i listan, eller nullptr om listan är tom
		DoublyLinkedListNode<T>* GetLast();

		// Skjuter in ett nytt element först i listan
		void InsertFirst(const T& aValue);

		// Skjuter in ett nytt element sist i listan
		void InsertLast(const T& aValue);

		// Skjuter in ett nytt element innan aNode
		void InsertBefore(DoublyLinkedListNode<T>* aNode, const T& aValue);

		// Skjuter in ett nytt element efter aNode
		void InsertAfter(DoublyLinkedListNode<T>* aNode, const T& aValue);

		// Plockar bort noden ur listan och frigör minne. (Det är ok att anta att
		// aNode är en nod i listan, och inte från en annan lista)
		void Remove(DoublyLinkedListNode<T>* aNode);

		// Hittar första elementet i listan som har ett visst värde. Jämförelsen
		// görs med operator==. Om inget element hittas returneras nullptr.
		DoublyLinkedListNode<T>* FindFirst(const T& aValue);

		// Hittar sista elementet i listan som har ett visst värde. Jämförelsen
		// görs med operator==. Om inget element hittas returneras nullptr.
		DoublyLinkedListNode<T>* FindLast(const T& aValue);

		// Plockar bort första elementet i listan som har ett visst värde.
		// Jämförelsen görs med operator==. Om inget element hittas görs ingenting.
		// Returnerar true om ett element plockades bort, och false annars.
		bool RemoveFirst(const T& aValue);

		// Plockar bort sista elementet i listan som har ett visst värde.
		// Jämförelsen görs med operator==. Om inget element hittas görs ingenting.
		// Returnerar true om ett element plockades bort, och false annars.
		bool RemoveLast(const T& aValue);

	private:
		DoublyLinkedListNode<T>* myFirst = nullptr;
		DoublyLinkedListNode<T>* myLast = nullptr;

	};

	template<class T>
	inline DoublyLinkedList<T>::DoublyLinkedList()
	{
		myFirst = nullptr;
		myLast = nullptr;
	}

	template<class T>
	inline DoublyLinkedList<T>::~DoublyLinkedList()
	{
		if (GetSize() == 0)
		{
			return;
		}

		DoublyLinkedListNode<T>* currentNode = myFirst;
		while (currentNode->myNext != nullptr)
		{
			currentNode = currentNode->GetNext();
			delete currentNode->myPrevious;
			currentNode->myPrevious = nullptr;
		}
		delete myLast;
		myFirst = nullptr;
		myLast = nullptr;
	}

	template<class T>
	inline int DoublyLinkedList<T>::GetSize() const
	{
		if (myFirst == nullptr)
		{
			return 0;
		}

		DoublyLinkedListNode<T>* node = myFirst;
		int counter = 0;
		while(node != nullptr)
		{
			node = node->GetNext();
			counter++;
		}
		return counter;
	}

	template<class T>
	inline DoublyLinkedListNode<T>* DoublyLinkedList<T>::GetFirst()
	{
		return myFirst;
	}

	template<class T>
	inline DoublyLinkedListNode<T>* DoublyLinkedList<T>::GetLast()
	{
		return myLast;
	}

	template<class T>
	inline void DoublyLinkedList<T>::InsertFirst(const T& aValue)
	{
		DoublyLinkedListNode<T>* node = new DoublyLinkedListNode<T>(aValue);

		if (myFirst == nullptr)
		{
			myFirst = node;
			myLast = node;
		}
		else
		{
			node->myNext = myFirst;
			myFirst->myPrevious = node;
			myFirst = node;
		}
	}

	template<class T>
	inline void DoublyLinkedList<T>::InsertLast(const T& aValue)
	{	
		DoublyLinkedListNode<T>* node = new DoublyLinkedListNode<T>(aValue);

		if (myLast == nullptr)
		{
			myLast = node;
			myFirst = node;
		}
		else
		{
			node->myPrevious = myLast;
			myLast->myNext = node;
			myLast = node;
		}
	}

	template<class T>
	inline void DoublyLinkedList<T>::InsertBefore(DoublyLinkedListNode<T>* aNode, const T& aValue)
	{
		assert(aNode != nullptr && "Node is nullptr");

		DoublyLinkedListNode<T>* newNode = new DoublyLinkedListNode<T>(aValue);

		if(myFirst == aNode)
		{
			newNode->myNext = myFirst;
			myFirst->myPrevious = newNode;
			myFirst = newNode;
		}
		else
		{
			newNode->myNext = aNode;
			newNode->myPrevious = aNode->GetPrevious();

			newNode->myPrevious->myNext = newNode;
			newNode->myNext->myPrevious = newNode;
		}
	}

	template<class T>
	inline void DoublyLinkedList<T>::InsertAfter(DoublyLinkedListNode<T>* aNode, const T& aValue)
	{
		assert(aNode != nullptr && "Node is nullptr");

		DoublyLinkedListNode<T>* newNode = new DoublyLinkedListNode<T>(aValue);

		if (myLast == aNode)
		{
			newNode->myPrevious = myLast;
			myLast->myNext = newNode;
			myLast = newNode;
		}
		else
		{
			newNode->myPrevious = aNode;
			newNode->myNext = aNode->GetNext();

			newNode->myPrevious->myNext = newNode;
			newNode->myNext->myPrevious = newNode;
		}
	}

	template<class T>
	inline void DoublyLinkedList<T>::Remove(DoublyLinkedListNode<T>* aNode)
	{
		assert(aNode != nullptr && "Node is nullptr");
		assert(GetSize() > 0 && "Can't remove elements from an empty list.");

		if (myFirst == aNode)
		{
			DoublyLinkedListNode<T>* next = myFirst->GetNext();
			delete myFirst;
			myFirst = next;
		}
		else if (myLast == aNode)
		{
			DoublyLinkedListNode<T>* previous = myLast->GetPrevious();
			delete myLast;
			myLast = previous;
		}
		else if (aNode != nullptr)
		{
			aNode->myPrevious->myNext = aNode->myNext;
			aNode->myNext->myPrevious = aNode->myPrevious;
			delete aNode;
		}
	}

	template<class T>
	inline DoublyLinkedListNode<T>* DoublyLinkedList<T>::FindFirst(const T& aValue)
	{
		DoublyLinkedListNode<T>* node = myFirst;
		while(node != nullptr)
		{
			if(node->GetValue() == aValue)
			{
				return node;
			}
			node = node->GetNext();
		}
		return nullptr;
	}

	template<class T>
	inline DoublyLinkedListNode<T>* DoublyLinkedList<T>::FindLast(const T& aValue)
	{
		DoublyLinkedListNode<T>* node = myLast;
		while(node != nullptr)
		{
			if(node->GetValue() == aValue)
			{
				return node;
			}
			node = node->GetPrevious();
		}
		return nullptr;
	}

	template<class T>
	inline bool DoublyLinkedList<T>::RemoveFirst(const T& aValue)
	{
		assert(GetSize() > 0 && "Can't remove elements from an empty list.");

		DoublyLinkedListNode<T>* node = myFirst;

		while(node != nullptr)
		{
			if(node->GetValue() == aValue)
			{
				Remove(node);
				return true;
			}
			node = node->GetNext();
		}
		return false;
	}

	template<class T>
	inline bool DoublyLinkedList<T>::RemoveLast(const T& aValue)
	{
		assert(GetSize() > 0 && "Can't remove elements from an empty list.");

		DoublyLinkedListNode<T>* node = myLast;

		while (node != nullptr)
		{
			if (node->GetValue() == aValue)
			{
				Remove(node);
				return true;
			}
			node = node->GetPrevious();
		}
		return false;
	}
}
namespace CU = CommonUtilities;