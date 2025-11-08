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
