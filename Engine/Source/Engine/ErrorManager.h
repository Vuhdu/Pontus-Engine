#pragma once

#define INFO_PRINT(aFormat, ...) CEngine::GetErrorManager()->InfoPrint(aFormat, ##__VA_ARGS__);
#define WARNING_PRINT(aFormat, ...) CEngine::GetErrorManager()->WarningPrint(__FILE__, __LINE__, aFormat, ##__VA_ARGS__);
#define ERROR_PRINT(aFormat, ...) CEngine::GetErrorManager()->ErrorPrint(__FILE__, __LINE__, aFormat, ##__VA_ARGS__);

class CErrorManager
{
public:
	CErrorManager() = default;
	~CErrorManager() = default;

	void InfoPrint(const char* aFormat, ...);
	void WarningPrint(const char* aFile, int aLine, const char* aFormat, ...);
	void ErrorPrint(const char* aFile, int aLine, const char* aFormat, ...);

	void SetConsoleColor(int aColor);

private:

};
