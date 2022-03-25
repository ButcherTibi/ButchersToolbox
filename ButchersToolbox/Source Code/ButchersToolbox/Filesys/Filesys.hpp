#pragma once

// Windows
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// Standard
#include <cstdint>
#include <string>
#include <vector>

// Mine
#include "../Windows/WindowsSpecific.hpp"


namespace filesys {

	void getExecutablePath(std::string& exe_path);


	template<typename T = char>
	void readFile(std::string& path, std::vector<T>& r_bytes)
	{
		if (path.empty()) {
			__debugbreak();
		}

		win32::Handle file_handle = CreateFile(path.c_str(),
			GENERIC_READ, // desired acces
			0,  // share mode
			NULL,  // security atributes
			OPEN_EXISTING,  // disposition
			FILE_FLAG_SEQUENTIAL_SCAN, // flags and atributes
			NULL  // template
		);

		if (file_handle.isValid() == false) {

			// failed to a create file handle
			__debugbreak();
		}

		// find file size
		LARGE_INTEGER file_size;
		if (GetFileSizeEx(file_handle.handle, &file_size) == false) {

			// failed to find file size
			__debugbreak();
		}
		r_bytes.resize(file_size.QuadPart);

		// read file
		DWORD bytes_read;

		auto result = ReadFile(
			file_handle.handle,
			r_bytes.data(),
			(DWORD)file_size.QuadPart,
			&bytes_read,
			NULL
		);

		if (result == false) {

			// failed to read file
			__debugbreak();
		}
	}

	template<typename T = char>
	void writeFile(std::string& path, std::vector<T>& bytes)
	{
		if (path.empty()) {
			__debugbreak();
		}

		win32::Handle file_handle = CreateFile(path.c_str(),
			GENERIC_WRITE, // desired acces
			0,  // share mode
			NULL,  // security atributes
			OPEN_ALWAYS,  // disposition
			FILE_FLAG_SEQUENTIAL_SCAN, // flags and atributes
			NULL  // template
		);

		if (file_handle.isValid() == false) {

			// failed to a create file handle
			__debugbreak();
		}

		DWORD bytes_writen;

		auto result = WriteFile(
			file_handle.handle,
			bytes.data(),
			(DWORD)bytes.size(),
			&bytes_writen,
			NULL
		);

		if (result == false) {

			// failed to read file
			__debugbreak();
		}
	}


	class Path {
	public:
		std::vector<std::string> entries;

	public:
		void _pushPathToEntries(std::string& path);

		Path() = default;
		Path(std::string path);

		void append(std::string entry);
		void pop(uint32_t count = 1);

		std::string toString(char separator = '\\');
	};
}
