#pragma once
#include "BSTSetNode.hpp"

namespace CommonUtilities
{
	template <class T>
	class BSTSet
	{
	public:
		BSTSet();
		~BSTSet();

		//Checks if the tree contains a value.
		bool HasElement(const T& aValue);

		//Inserts an a value into the tree.
		void Insert(const T& aValue);

		//Removes a value from the tree.
		void Remove(const T& aValue);
		
		void DSWBalance();

	private:
		BSTSetNode<T>* Search(BSTSetNode<T>* aNode, const T& aValue) const;
		void DeleteTree(BSTSetNode<T>* aNode);
		BSTSetNode<T>* DeleteNode(BSTSetNode<T>* aNode, const T& aValue);

		BSTSetNode<T>* GetRightMostNode(BSTSetNode<T>* aNode);

		void CreateBackbone(BSTSetNode<T>* aRoot);
		void CreatePerfectTree();
		void Compact(int aIndex);

		void RotateRight(BSTSetNode<T>* aGrNode, BSTSetNode<T>* aPrNode, BSTSetNode<T>* aChNode);
		void RotateLeft(BSTSetNode<T>* aGrNode, BSTSetNode<T>* aPrNode, BSTSetNode<T>* aChNode);

	private:
		BSTSetNode<T>* myRoot;
		int myNumberOfNodes = 0;
	};

	template<class T>
	inline BSTSet<T>::BSTSet()
	{
		myRoot = nullptr;
		myNumberOfNodes = 0;
	}

	template<class T>
	inline BSTSet<T>::~BSTSet()
	{
		DeleteTree(myRoot);
		myNumberOfNodes = 0;
		myRoot = nullptr;
	}

	template<class T>
	inline void BSTSet<T>::DeleteTree(BSTSetNode<T>* aNode)
	{
		if (aNode == nullptr)
		{
			return;
		}

		DeleteTree(aNode->myLeft);
		DeleteTree(aNode->myRight);

		delete aNode;
	}

	template<class T>
	inline bool BSTSet<T>::HasElement(const T& aValue)
	{
		BSTSetNode<T>* nodeToCheck = Search(myRoot, aValue);
		return nodeToCheck != nullptr && nodeToCheck->GetData() == aValue;
	}

	template<class T>
	inline void BSTSet<T>::Insert(const T& aValue)
	{
		if (myRoot == nullptr)
		{
			myRoot = new BSTSetNode<T>(aValue);
			return;
		}

		BSTSetNode<T>* node = Search(myRoot, aValue);
		if(!(node->GetData() < aValue) && !(aValue < node->GetData()))
		{
			return;
		}
		else if(aValue < node->GetData())
		{
			node->myLeft = new BSTSetNode<T>(aValue);
			myNumberOfNodes++;
		}
		else
		{
			node->myRight = new BSTSetNode<T>(aValue);
			myNumberOfNodes++;
		}
	}

	template<class T>
	inline void BSTSet<T>::Remove(const T& aValue)
	{
		myRoot = DeleteNode(myRoot, aValue);
	}

	template<class T>
	inline void BSTSet<T>::DSWBalance()
	{
		CreateBackbone(myRoot);
		CreatePerfectTree();
	}

	template<class T>
	inline void BSTSet<T>::CreateBackbone(BSTSetNode<T>* aRoot)
	{
		BSTSetNode<T>* parent = aRoot;
		BSTSetNode<T>* grandParent = nullptr;

		while (parent != nullptr)
		{
			if(parent->myLeft != nullptr)
			{
				BSTSetNode<T>* child = parent->myLeft;
				RotateRight(grandParent, parent, child);
				parent = child;
			}
			else
			{
				grandParent = parent;
				parent = parent->myRight;
			}
		}
	}

	template<class T>
	inline void BSTSet<T>::CreatePerfectTree()
	{
		int m = (1 << ((int)(log10(myNumberOfNodes + 1) / log10(2)))) - 1;
		
		Compact(myNumberOfNodes - m);

		while (m > 1)
		{
			m /= 2;
			Compact(m);
		}
	}

	template<class T>
	inline void BSTSet<T>::Compact(int aNumberOfRotations)
	{
		BSTSetNode<T>* grandParent = nullptr;
		BSTSetNode<T>* parent = myRoot;

		for (int i = 0; i < aNumberOfRotations; i++)
		{
			BSTSetNode<T>* child = parent->myRight;

			RotateLeft(grandParent, parent, child);
			grandParent = child;
			parent = child->myRight;
		}
	}

	template<class T>
	inline void BSTSet<T>::RotateRight(BSTSetNode<T>* aGrNode, BSTSetNode<T>* aPrNode, BSTSetNode<T>* aChNode)
	{
		if (aGrNode != nullptr)
		{
			aGrNode->myRight = aChNode;
		}
		else
		{
			myRoot = aChNode;
		}
		aPrNode->myLeft = aChNode->myRight;
		aChNode->myRight = aPrNode;
	}

	template<class T>
	inline void BSTSet<T>::RotateLeft(BSTSetNode<T>* aGrNode, BSTSetNode<T>* aPrNode, BSTSetNode<T>* aChNode)
	{
		if(aGrNode != nullptr)
		{
			aGrNode->myRight = aChNode;
		}
		else
		{
			myRoot = aChNode;
		}
		aPrNode->myRight = aChNode->myLeft;
		aChNode->myLeft = aPrNode;
	}

	template<class T>
	inline BSTSetNode<T>* BSTSet<T>::Search(BSTSetNode<T>* aNode, const T& aValue) const
	{		
		if(aNode == nullptr)
		{
			return aNode;
		}
		else if(!(aNode->GetData() < aValue) && !(aValue < aNode->GetData()))
		{
			return aNode;
		}
		else if(aValue < aNode->GetData() && aNode->GetLeft() == nullptr)
		{
			return aNode;
		}
		else if(aNode->GetData() < aValue && aNode->GetRight() == nullptr)
		{
			return aNode;
		}

		if(aValue < aNode->GetData())
		{
			return Search(aNode->myLeft, aValue);
		}
		return Search(aNode->myRight, aValue);
	}

	template<class T>
	inline BSTSetNode<T>* BSTSet<T>::DeleteNode(BSTSetNode<T>* aNode, const T& aValue)
	{		
		if (aNode == nullptr)
		{
			return aNode;
		}
        
		if (aValue < aNode->GetData())
		{
			aNode->myLeft = DeleteNode(aNode->myLeft, aValue);
		}
		else if (aNode->GetData() < aValue)
		{
			aNode->myRight = DeleteNode(aNode->myRight, aValue);
		}
		else
		{
			BSTSetNode<T>* nodeToBeRecoupled;

			if (aNode->myLeft == nullptr)
			{
				nodeToBeRecoupled = aNode->myRight;
				myNumberOfNodes--;
				delete aNode;
				return nodeToBeRecoupled;
			}
			else if (aNode->myRight == nullptr)
			{
				nodeToBeRecoupled = aNode->myLeft;
				myNumberOfNodes--;
				delete aNode;
				return nodeToBeRecoupled;
			}
			
			nodeToBeRecoupled = GetRightMostNode(aNode->myLeft);
			aNode->myData = nodeToBeRecoupled->myData;
			aNode->myLeft = DeleteNode(aNode->myLeft, nodeToBeRecoupled->myData);
		}
		return aNode;		
	}

	template<class T>
	inline BSTSetNode<T>* BSTSet<T>::GetRightMostNode(BSTSetNode<T>* aNode)
	{
		BSTSetNode<T>* currentNode = aNode;

		while (currentNode != nullptr && currentNode->myRight != nullptr)
		{
			currentNode = currentNode->myRight;
		}
		return currentNode;
	}
	
}
namespace CU = CommonUtilities;