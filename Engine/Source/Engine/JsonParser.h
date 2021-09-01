#pragma once
#include "nlohmann/json.hpp"
#include <map>
#include <string>

class JsonParser
{
public:
	static JsonParser& GetInstance()
	{
		static JsonParser instance;
		return instance;
	}
	void Init();

	nlohmann::json GetDataFromIDString(const std::string& aID) const;
	nlohmann::json GetDataFromFile(const std::string& aFilePath) const;

private:
	JsonParser() {};
	JsonParser(const JsonParser&) = delete;
	void operator=(const JsonParser&) = delete;

	std::map<std::string, nlohmann::json> myCachedData;
};

