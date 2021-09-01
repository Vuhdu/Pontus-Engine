#pragma once
#include <vector>
#include <assert.h>

namespace CommonUtilities
{
	enum class eState
	{
		Empty = 0,
		InUse = 1,
		Removed = 2
	};

	template <class Key, class Value>
	struct Data
	{
		eState myState = eState::Empty;
		Key myKey;
		Value myValue;
	};

	template <class Key, class Value>
	class HashMap
	{
	public:
		//Skapar en HashMap med plats för aCapacity element. Den behöver inte
		//kunna växa. Blir den full så blir den full.
		HashMap(int aCapacity);

		//Stoppar in aValue under nyckeln aKey. Om det fanns ett värde innan med
		//aKey som nyckel så skrivs det värdet över. Om det lyckas returneras
		//true. Om map:en är full så görs ingenting, och false returneras.
		bool Insert(const Key& aKey, const Value& aValue);

		//Plockar bort elementet med nyckeln aKey, om det finns. Om det finns
		//returneras true. Om det inte finns görs ingenting, och false
		//returneras.
		bool Remove(const Key& aKey);

		//Ger en pekare till värdet med nyckeln aKey, om det finns. Om det inte
		//finns returneras nullptr.
		const Value* Get(const Key& aKey) const;

		//Som ovan, men returnerar en icke-const-pekare.
		Value* Get(const Key& aKey);

	private:
		void GetElement(const Key& aKey, unsigned int& aIncrement, const unsigned int aStartIndex, bool& aHasElementFlag);

	private:
		const int myCapacity;
		std::vector<Data<Key, Value>> myEntries;

	};

	uint32_t Hash(const uint8_t* aBuffer, int aCount)
	{
		const uint32_t FNVOffsetBasis = 2166136261U;
		const uint32_t FNVPrime = 16777619U;
		uint32_t val = FNVOffsetBasis;
	
		for(int i = 0; i < aCount; ++i)
		{
			val ^= aBuffer[i];
			val *= FNVPrime;
		}
		return val;
	}

	template <class Key>
	int HashIndex(const Key& aKey, const int aSize)
	{
		return Hash(aKey) % aSize;
	}

	template <class Key>
	uint32_t Hash(const Key& aKey)
	{
		return Hash(reinterpret_cast<const uint8_t*>(&aKey), sizeof(Key));
	}

	uint32_t Hash(const std::string& aString)
	{
		return Hash(reinterpret_cast<const uint8_t*>(aString.c_str()), aString.size());
	}

	template<class Key, class Value>
	inline HashMap<Key, Value>::HashMap(int aCapacity) : myCapacity(aCapacity)
	{
		myEntries.reserve(myCapacity);
		myEntries.resize(myCapacity);
	}

	template<class Key, class Value>
	inline bool HashMap<Key, Value>::Insert(const Key& aKey, const Value& aValue)
	{
		if (myCapacity <= 0)
		{
			return false;
		}
		unsigned int startIndex = HashIndex(aKey, myEntries.size());
		unsigned int increment = startIndex;
		bool foundElement = false;
		bool notLooped = true;

		while (notLooped)
		{
			if (myEntries[increment].myState == eState::InUse || myEntries[increment].myState == eState::Removed)
			{
				if ((myEntries[increment].myKey < aKey) == false && (aKey < myEntries[increment].myKey) == false)
				{
					foundElement = true;
					break;
				}
			}
			else if (myEntries[increment].myState == eState::Empty)
			{
				foundElement = true;
				break;
			}

			++increment;
			if (increment >= myCapacity)
			{
				increment = 0;
			}

			if (increment == startIndex)
			{
				notLooped = false;
			}
		}

		if (foundElement == true)
		{
			myEntries[increment].myState = eState::InUse;
			myEntries[increment].myValue = aValue;
			myEntries[increment].myKey = aKey;

			return true;
		}
		
		return false;
	}

	template<class Key, class Value>
	inline bool HashMap<Key, Value>::Remove(const Key& aKey)
	{
		unsigned int startIndex = HashIndex(aKey, myEntries.size());
		unsigned int increment = startIndex;
		bool foundElement = false;

		GetElement(aKey, increment, startIndex, foundElement);

		if (foundElement == true)
		{
			myEntries[increment].myState = eState::Removed;
			return true;
		}
		return false;
	}

	template<class Key, class Value>
	inline const Value* HashMap<Key, Value>::Get(const Key& aKey) const
	{
		unsigned int startIndex = HashIndex(aKey, myEntries.size());
		unsigned int increment = startIndex;
		bool foundElement = false;

		GetElement(aKey, increment, startIndex, foundElement);

		if (foundElement == true)
		{
			return &myEntries[increment].myValue;
		}
		return nullptr;
	}

	template<class Key, class Value>
	inline Value* HashMap<Key, Value>::Get(const Key& aKey)
	{
		unsigned int startIndex = HashIndex(aKey, myEntries.size());
		unsigned int increment = startIndex;
		bool foundElement = false;

		GetElement(aKey, increment, startIndex, foundElement);

		if (foundElement == true)
		{
			return &myEntries[increment].myValue;
		}
		return nullptr;
	}

	template<class Key, class Value>
	inline void HashMap<Key, Value>::GetElement(const Key& aKey, unsigned int& aIncrement, const unsigned int aStartIndex, bool& aHasElementFlag)
	{
		bool notLooped = true;
		while (notLooped)
		{
			if (myEntries[aIncrement].myState == eState::InUse)
			{
				if ((myEntries[aIncrement].myKey < aKey) == false && (aKey < myEntries[aIncrement].myKey) == false)
				{
					aHasElementFlag = true;
					break;
				}
			}
			else if (myEntries[aIncrement].myState == eState::Empty)
			{
				notLooped = false;
			}

			++aIncrement;
			if (aIncrement >= myCapacity)
			{
				aIncrement = 0;
			}
			if (aIncrement == aStartIndex)
			{
				notLooped = false;
			}
		}
	}
}
namespace CU = CommonUtilities;

