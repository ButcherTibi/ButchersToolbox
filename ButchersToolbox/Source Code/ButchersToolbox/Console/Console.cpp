#include "Console.hpp"

// Windows
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <cassert>


void Console::_setForegroundColor(uint8_t red, uint8_t green, uint8_t blue)
{
	printf("\x1b[38;2;%d;%d;%dm", red, green, blue);
}

void Console::_setBackgroundColor(uint8_t red, uint8_t green, uint8_t blue)
{
	printf("\x1b[48;2;%d;%d;%dm", red, green, blue);
}

void Console::_setUnderline(bool enable_underline)
{
	if (enable_underline) {
		printf("\x1b[4m");
	}
	else {
		printf("\x1b[24m");
	}
}

void Console::configureForUTF8()
{
	SetConsoleOutputCP(CP_UTF8);
}

void Console::enableVirtualProcessing()
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

	DWORD dwMode = 0;
	GetConsoleMode(handle, &dwMode);

	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(handle, dwMode);
}

void Console::setForegroundColor(uint8_t red, uint8_t green, uint8_t blue)
{
	Console::current_formating.foreground_color = { red, green, blue };
	printf("\x1b[38;2;%d;%d;%dm", red, green, blue);
}

void Console::setBackgroundColor(uint8_t red, uint8_t green, uint8_t blue)
{
	Console::current_formating.background_color = { red, green, blue };
	printf("\x1b[48;2;%d;%d;%dm", red, green, blue);
}

void Console::setUnderline(bool enable_underline)
{
	Console::current_formating.enable_underline = enable_underline;
	Console::_setUnderline(enable_underline);
}

void Console::resetFormatting()
{
	printf("\x1b[0m");
}

void Console::getCurrentFolder(std::string& current_folder)
{
	current_folder.resize(GetCurrentDirectoryA(0, nullptr));
	GetCurrentDirectoryA((uint32_t)current_folder.size(), current_folder.data());

	// don't need \\0
	current_folder.pop_back();
}
