#include "stdafx.h"
#include "ErrorManager.h"
#include <iostream>
#include <cstdarg>
#include <Windows.h>

#define CONSOLE_TEXT_COLOR_RED 12
#define CONSOLE_TEXT_COLOR_YELLOW 14
#define CONSOLE_TEXT_COLOR_GREEN 10
#define CONSOLE_TEXT_COLOR_WHITE 15

void CErrorManager::SetConsoleColor(int aColor)
{
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (!hConsole)
	{
		return;
	}
	SetConsoleTextAttribute(hConsole, static_cast<WORD>(aColor));
}

void CErrorManager::InfoPrint(const char* aFormat, ...)
{
	SetConsoleColor(CONSOLE_TEXT_COLOR_GREEN);

	va_list argptr;
	va_start(argptr, aFormat);

	vfprintf(stderr, aFormat, argptr);
	va_end(argptr);
	std::cout << std::endl;
}

void CErrorManager::WarningPrint(const char* aFile, int aLine, const char* aFormat, ...)
{
	SetConsoleColor(CONSOLE_TEXT_COLOR_YELLOW);

	std::string file = std::string(aFile);
	const size_t last_slash_idx = file.find_last_of("\\/");
	if (std::string::npos != last_slash_idx)
	{
		file.erase(0, last_slash_idx + 1);
	}

	printf("Warning in %s at line: %i ", file.c_str(), aLine);

	va_list argptr;
	va_start(argptr, aFormat);

	vfprintf(stderr, aFormat, argptr);
	va_end(argptr);
	std::cout << std::endl;
}

void CErrorManager::ErrorPrint(const char* aFile, int aLine, const char* aFormat, ...)
{
	SetConsoleColor(CONSOLE_TEXT_COLOR_RED);

	std::string file = std::string(aFile);
	const size_t last_slash_idx = file.find_last_of("\\/");
	if (std::string::npos != last_slash_idx)
	{
		file.erase(0, last_slash_idx + 1);
	}

	printf("Error in %s at line: %i ", file.c_str(), aLine);

	va_list argptr;
	va_start(argptr, aFormat);

	vfprintf(stderr, aFormat, argptr);
	va_end(argptr);
	std::cout << std::endl;
}
