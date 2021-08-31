#pragma once

namespace CommonUtilities
{
	template <class T>
	class BSTSet;

	template <class T>
	class BSTSetNode
	{
	public:
		BSTSetNode(const T& aValue);
		~BSTSetNode();

		const T&				GetData() const;
		const BSTSetNode*		GetLeft() const;
		const BSTSetNode*		GetRight() const;

	private:
		friend class BSTSet<T>;

		T myData;
		BSTSetNode* myLeft = nullptr;
		BSTSetNode* myRight = nullptr;

	};

	template<class T>
	inline BSTSetNode<T>::BSTSetNode(const T& aValue)
	{
		myLeft = nullptr;
		myRight = nullptr;
		myData = aValue;
	}

	template<class T>
	inline BSTSetNode<T>::~BSTSetNode()
	{
	}

	template<class T>
	inline const T& BSTSetNode<T>::GetData() const
	{
		return myData;
	}

	template<class T>
	inline const BSTSetNode<T>* BSTSetNode<T>::GetLeft() const
	{
		return myLeft;
	}

	template<class T>
	inline const BSTSetNode<T>* BSTSetNode<T>::GetRight() const
	{
		return myRight;
	}
}
namespace CU = CommonUtilities;