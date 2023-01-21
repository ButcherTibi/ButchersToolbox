
// Header
#include "WindowsSpecific.hpp"

using namespace win32;


inline void assert_cond(bool condition) {
#ifndef NDEBUG  // or _DEBUG
	if (condition != true) {
		throw std::exception();
	}
#endif
}

inline void assert_cond(bool condition, const char* fail_msg) {
#ifndef NDEBUG  // or _DEBUG
	if (condition != true) {
		throw std::exception(fail_msg);
	}
#endif
}

#define code_location \
	("ln = " + std::to_string(__LINE__) + \
	" fn = " + __func__ + \
	" in file " + __FILE__).c_str()

void win32::check(BOOL function_call_value)
{
	if (function_call_value == false) {
		__debugbreak();
	}
}

std::wstring win32::getLastError()
{
	LPWSTR buffer;

	FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		0,
		(LPWSTR)&buffer,
		0,
		NULL
	);

	std::wstring error_msg = buffer;

	LocalFree(buffer);

	return error_msg;
}

Handle::Handle()
{
	this->handle = INVALID_HANDLE_VALUE;
}

Handle::Handle(HANDLE ms_handle)
{
	close();
	this->handle = ms_handle;
}

Handle& Handle::operator=(HANDLE ms_handle)
{
	close();
	this->handle = ms_handle;
	return *this;
}

bool Handle::isValid()
{
	void* f = (void*)0xFFFF'FFFF'FFFF'FFFF;

	if (handle == INVALID_HANDLE_VALUE) {
		return false;
	}
	else if (handle == f) {
		return false;
	}

	return true;
}

void Handle::close()
{
	if (isValid()) {
		CloseHandle(handle);
	}
}

Handle::~Handle()
{
	close();
}

void win32::printToOutput(std::wstring message)
{
	OutputDebugStringW(message.c_str());
}
