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

	class Path {
	public:
		std::vector<std::string> entries;

		void _pushPathToEntries(const std::string& path);

	public:
		Path() = default;
		Path(std::string string_path);

		// Operators
		Path operator=(std::string string_path);

		// Statics
		static Path executablePath();

		// Modification
		void append(std::string entry);
		void pop(uint32_t count = 1);

		// File Read
		template<typename T = char>
		void readFile(std::vector<T>& r_bytes)
		{
			win32::Handle file_handle = CreateFile(toString().c_str(),
				GENERIC_READ, // desired acces
				0,  // share mode
				NULL,  // security atributes
				OPEN_EXISTING,  // disposition
				FILE_FLAG_SEQUENTIAL_SCAN, // flags and atributes
				NULL  // template
			);

			if (file_handle.isValid() == false) {
				__debugbreak();
			}

			// find file size
			LARGE_INTEGER file_size;
			if (GetFileSizeEx(file_handle.handle, &file_size) == false) {
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
				__debugbreak();
			}
		}

		// File Write
		template<typename T = char>
		void writeFile(std::vector<T>& bytes)
		{
			win32::Handle file_handle = CreateFile(toString().c_str(),
				GENERIC_WRITE, // desired acces
				0,  // share mode
				NULL,  // security atributes
				OPEN_ALWAYS,  // disposition
				FILE_FLAG_SEQUENTIAL_SCAN, // flags and atributes
				NULL  // template
			);

			if (file_handle.isValid() == false) {
				__debugbreak();
			}

			DWORD bytes_writen;

			auto result = WriteFile(
				file_handle.handle,
				bytes.data(),
				(DWORD)bytes.size(),
				&bytes_writen,
				NULL  // overllaped
			);

			if (result == false) {
				__debugbreak();
			}
		}

		// Export
		std::string toString(char separator = '\\');
	};
}
