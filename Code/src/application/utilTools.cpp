#include "pch.h"
#include "utilTools.h"

#include <iostream>
#include <Windows.h>


void DebugLog(const std::string& message, DebugType type)
{
#ifdef _WIN32
	// Optional color (green) for readability; reset after printing.
	WORD color;
	switch (type)
	{
	case Message:
		color = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		break;
	case Warning:
		color = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		break;
	case Error:
		color = FOREGROUND_RED | FOREGROUND_INTENSITY;
		break;
	default:
		color = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		break;
	}

	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	if (h && h != INVALID_HANDLE_VALUE)
		SetConsoleTextAttribute(h, WORD(color | FOREGROUND_INTENSITY));
#endif

	std::cout << "[DEBUG] " << message << std::endl;

#ifdef _WIN32
	if (h && h != INVALID_HANDLE_VALUE)
		SetConsoleTextAttribute(h, WORD(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE));
#endif
}

std::string GetExecutableDirectory()
{
#ifdef _WIN32
	char buffer[MAX_PATH];
	DWORD length = GetModuleFileNameA(NULL, buffer, MAX_PATH);
	if (length > 0)
	{
		std::string exePath(buffer, length);
		size_t lastSlash = exePath.find_last_of("\\/");
		if (lastSlash != std::string::npos)
		{
			return exePath.substr(0, lastSlash + 1);
		}
	}
#endif
	// Fallback to current directory
	return "./";
}
