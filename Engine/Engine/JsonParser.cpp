#include "stdafx.h"
#include "JsonParser.h"
#include <fstream>

void JsonParser::Init()
{
	std::ifstream in("Assets/Data/CachedDataLocations.json");
	nlohmann::json cachedPaths;
	in >> cachedPaths;

	for (auto PathData = cachedPaths.begin(); PathData != cachedPaths.end(); ++PathData)
	{
		nlohmann::json cacheData;
		std::string path = PathData.value();
		std::ifstream n(path);
		n >> cacheData;

		auto& id = PathData.key();

		myCachedData[id] = cacheData;
	}
}

nlohmann::json JsonParser::GetDataFromIDString(const std::string& aID) const
{
	return myCachedData.at(aID);
}

nlohmann::json JsonParser::GetDataFromFile(const std::string& aFilePath) const
{
	std::ifstream data(aFilePath);
	nlohmann::json dataValue;
	data >> dataValue;
	return dataValue;
}