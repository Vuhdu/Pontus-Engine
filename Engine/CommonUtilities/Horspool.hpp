#pragma once
#include <string>
#include <array>

namespace CommonUtilities
{
	int Horspool(const std::string& aNeedle, const std::string& aHaystack)
	{
		int needleLength = aNeedle.length();
		int haystackLength = aHaystack.length();

		std::array<int, 256> table;

		table.fill(needleLength);
		for (int i = 0; i < needleLength - 1; i++)
		{
			table[static_cast<unsigned char>(aNeedle[i])] = needleLength - 1 - i;
		}

		int haystackIndex = needleLength - 1;

		while (haystackIndex <= haystackLength - 1)
		{
			int needleIndex = 0;

			while (needleIndex <= needleLength - 1 && aNeedle[needleLength - 1 - needleIndex] == aHaystack[haystackIndex - needleIndex])
			{
				needleIndex++;
			}

			if (needleIndex == needleLength)
			{
				return haystackIndex - needleLength + 1;
			}
			else
			{
				haystackIndex += table[static_cast<unsigned char>(aHaystack[haystackIndex])];
			}
		}
		return -1;
	}
}
namespace CU = CommonUtilities;