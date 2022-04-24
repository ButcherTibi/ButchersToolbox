#pragma once

// Standard
#include <cstdint>
#include <string>
#include <vector>

// Mine
#include "../Windows/WindowsSpecific.hpp"


namespace filesys {

	const uint32_t max_path_length = 1024;


#if UNICODE
	template<typename T = wchar_t>
#else
	template<typename T = char8_t>
#endif
	class Path {
		using string = std::basic_string<T>;

		std::vector<string> entries;

	private:
		void _pushPathToEntries(const string& path)
		{
			uint32_t i = 0;
			string entry;

			while (i < path.size()) {

				T chara = path[i];

				if (chara == '/' || chara == '\\') {

					if (entry.empty() == false) {

						entries.push_back(entry);
						entry.clear();
					}
				}
				else {
					entry.push_back(chara);

					if (i == path.size() - 1) {
						entries.push_back(entry);
					}
				}

				i++;
			}
		}

		void createFileHandle(win32::Handle& r_file_handle, size_t& r_file_size)
		{
			if constexpr (std::is_same<T, wchar_t>()) {
				r_file_handle = CreateFileW(toString().c_str(),
					GENERIC_READ, // desired acces
					0,  // share mode
					NULL,  // security atributes
					OPEN_EXISTING,  // disposition
					FILE_FLAG_SEQUENTIAL_SCAN, // flags and atributes
					NULL  // template
				);
			}
			else if constexpr (std::is_same<T, char>() || std::is_same<T, char8_t>()) {
				r_file_handle = CreateFileA((char*)toString().c_str(),
					GENERIC_READ, // desired acces
					0,  // share mode
					NULL,  // security atributes
					OPEN_EXISTING,  // disposition
					FILE_FLAG_SEQUENTIAL_SCAN, // flags and atributes
					NULL  // template
				);
			}

			if (r_file_handle.isValid() == false) {
				__debugbreak();
			}

			// find file size
			LARGE_INTEGER file_size;
			if (GetFileSizeEx(r_file_handle.handle, &file_size) == false) {
				__debugbreak();
			}
			r_file_size = file_size.QuadPart;
		}

		void createWriteFileHandle(win32::Handle& r_file_handle)
		{
			if constexpr (std::is_same<T, wchar_t>()) {
				r_file_handle = CreateFileW(toString().c_str(),
					GENERIC_WRITE, // desired acces
					0,  // share mode
					NULL,  // security atributes
					OPEN_ALWAYS,  // disposition
					FILE_FLAG_SEQUENTIAL_SCAN, // flags and atributes
					NULL  // template
				);
			}
			else if constexpr (std::is_same<T, char>() || std::is_same<T, char8_t>()) {
				r_file_handle = CreateFileA((char*)toString().c_str(),
					GENERIC_WRITE, // desired acces
					0,  // share mode
					NULL,  // security atributes
					OPEN_ALWAYS,  // disposition
					FILE_FLAG_SEQUENTIAL_SCAN, // flags and atributes
					NULL  // template
				);
			}

			if (r_file_handle.isValid() == false) {
				__debugbreak();
			}
		}

	public:
		Path() = default;
		Path(string string_path)
		{
			this->_pushPathToEntries(string_path);
		}

		// Operators
		Path operator=(string string_path)
		{
			this->_pushPathToEntries(string_path);
			return *this;
		}

		// Stats
		static Path executablePath()
		{
			string exe_path;
			exe_path.resize(max_path_length);

			uint32_t used_size;
			if constexpr (std::is_same<T, wchar_t>()) {
				used_size = GetModuleFileNameW(NULL, exe_path.data(), (uint32_t)exe_path.size());
			}
			else if constexpr (std::is_same<T, char>() || std::is_same<T, char8_t>()) {
				used_size = GetModuleFileNameA(NULL, (char*)exe_path.data(), (uint32_t)exe_path.size());
			}

			exe_path.resize(used_size);
			exe_path.shrink_to_fit();

			return exe_path;
		}

		// Modification
		void append(string entry)
		{
			_pushPathToEntries(entry);
		}

		void pop(uint32_t count = 1)
		{
			for (uint32_t i = 0; i < count; i++) {
				entries.pop_back();
			}
		}

		// File Read
		void read(std::vector<uint8_t>& r_bytes)
		{
			win32::Handle file_handle;
			size_t file_size;

			createFileHandle(file_handle, file_size);

			r_bytes.resize(file_size);

			// read file
			DWORD bytes_read;

			auto result = ReadFile(
				file_handle.handle,
				r_bytes.data(),
				(DWORD)file_size,
				&bytes_read,
				NULL
			);

			if (result == false) {
				__debugbreak();
			}
		}

		void read(std::string& r_text)
		{
			win32::Handle file_handle;
			size_t file_size;

			createFileHandle(file_handle, file_size);

			r_text.resize(file_size);

			// read file
			DWORD bytes_read;

			auto result = ReadFile(
				file_handle.handle,
				r_text.data(),
				(DWORD)file_size,
				&bytes_read,
				NULL
			);

			if (result == false) {
				__debugbreak();
			}
		}

		// File Write
		void write(std::vector<uint8_t>& bytes)
		{
			win32::Handle file_handle;
			createWriteFileHandle(file_handle);

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

		void write(std::string& text)
		{
			win32::Handle file_handle;
			createWriteFileHandle(file_handle);

			DWORD bytes_writen;

			auto result = WriteFile(
				file_handle.handle,
				text.data(),
				(DWORD)text.size(),
				&bytes_writen,
				NULL  // overllaped
			);

			if (result == false) {
				__debugbreak();
			}
		}

		// Export
		string toString(char separator = '\\')
		{
			string path;

			if (this->entries.size()) {

				size_t last = entries.size() - 1;
				for (size_t i = 0; i < entries.size(); i++) {

					for (auto& letter : entries[i]) {
						path.push_back(letter);
					}

					if (i != last) {
						path.push_back(separator);
					}
				}
			}
			return path;
		}
	};
}
