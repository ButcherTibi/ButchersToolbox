#pragma once

// Standard
#include <cstdint>
#include <string>
#include <vector>
#include <chrono>

// Mine
#include "../Windows/WindowsSpecific.hpp"


namespace filesys {

	const uint32_t max_path_length = 1024;


#if UNICODE
	template<typename T = wchar_t>
#else
	template<typename T = char>
#endif
	class Path {
		using string = std::basic_string<T>;
		std::vector<string> entries;

	private:
		void _pushPathToEntries(const string& path);

	public:
		Path() = default;
		Path(string string_path);

		// Operators
		Path operator=(string string_path);

		static Path executablePath();

		// Add
		void append(string entry);
		
		// Delete
		void pop(uint32_t count = 1);	

		// Export
		string toString(char separator = '\\');
	};


#if UNICODE
	template<typename T = wchar_t>
#else
	template<typename T = char>
#endif
	class File {
		using string = std::basic_string<T>;
		using time_point = std::chrono::time_point<std::chrono::system_clock>;

	private:
		static void createFile(string file_path,
			uint32_t access_flags, uint32_t share_mode, uint32_t disposition, uint32_t flags_and_atributes,
			win32::Handle& r_file_handle);

	public:
		/* Creation */

		File() = default;


		// File Read
		static void read(string file_path, void* mem);
		static void read(string file_path, std::vector<uint8_t>& r_bytes);
		static void read(string file_path, std::string& r_text);


		// File Write
		static void write(string file_path, size_t mem_size, void* mem);


		/* Read Atributes */

		static size_t size(string file_path);

		static std::chrono::time_point<std::chrono::system_clock> lastModifiedTime(string file_path);
	};


	template<typename T>
	void Path<T>::_pushPathToEntries(const string& path)
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

	template<typename T>
	Path<T>::Path(string string_path)
	{
		this->_pushPathToEntries(string_path);
	}

	template<typename T>
	Path<T> Path<T>::operator=(string string_path)
	{
		this->_pushPathToEntries(string_path);
		return *this;
	}

	template<typename T>
	Path<T> Path<T>::executablePath()
	{
		string exe_path;
		exe_path.resize(max_path_length);

		uint32_t used_size = GetModuleFileName(NULL, exe_path.data(), (uint32_t)exe_path.size());

		exe_path.resize(used_size);
		exe_path.shrink_to_fit();

		return exe_path;
	}

	template<typename T>
	void Path<T>::append(string entry)
	{
		_pushPathToEntries(entry);
	}

	template<typename T>
	void Path<T>::pop(uint32_t count)
	{
		for (uint32_t i = 0; i < count; i++) {
			entries.pop_back();
		}
	}

	template<typename T>
	std::basic_string<T> Path<T>::toString(char separator)
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

	template<typename T>
	void File<T>::createFile(string file_path,
		uint32_t access_flags, uint32_t share_mode, uint32_t disposition, uint32_t flags_and_atributes,
		win32::Handle& r_file)
	{
		if constexpr (std::is_same<T, wchar_t>()) {
			r_file.handle = CreateFileW(file_path.c_str(),
				access_flags, // desired acces
				share_mode,  // share mode
				NULL,  // security atributes
				disposition,  // disposition
				flags_and_atributes, // flags and atributes
				NULL  // template
			);
		}
		else if constexpr (std::is_same<T, char>() || std::is_same<T, char8_t>()) {
			r_file.handle = CreateFileA(file_path.c_str(),
				access_flags, // desired acces
				share_mode,  // share mode
				NULL,  // security atributes
				disposition,  // disposition
				flags_and_atributes, // flags and atributes
				NULL  // template
			);
		}
		else {
			__debugbreak();
		}

		if (r_file.isValid() == false) {
			win32::printToOutput(win32::getLastError());
			__debugbreak();
		}
	}

	template<typename T>
	void File<T>::read(string file_path, void* mem)
	{
		uint32_t file_size = (uint32_t)size(file_path);

		win32::Handle file_handle;
		createFile(file_path,
			GENERIC_READ,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			OPEN_EXISTING,
			FILE_FLAG_SEQUENTIAL_SCAN,
			file_handle
		);

		auto result = ReadFile(
			file_handle.handle,
			mem,
			file_size,
			nullptr,
			nullptr
		);

		if (result == false) {
			__debugbreak();
		}
	}

	template<typename T>
	void File<T>::read(string file_path, std::vector<uint8_t>& r_bytes)
	{
		r_bytes.resize(size(file_path));
		read(file_path, r_bytes.data());
	}

	template<typename T>
	void File<T>::read(string file_path, std::string& r_text)
	{
		r_text.resize(size(file_path));
		read(file_path, r_text.data());
	}

	template<typename T>
	void File<T>::write(string file_path, size_t mem_size, void* mem)
	{
		win32::Handle file;

		createFile(file_path, GENERIC_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, file);

		auto result = WriteFile(
			file.handle,
			mem,
			(uint32_t)mem_size,
			nullptr, nullptr
		);

		if (result == false) {
			__debugbreak();
		}
	}

	template<typename T>
	size_t File<T>::size(string file_path)
	{
		win32::Handle file_handle;
		createFile(file_path,
			GENERIC_READ,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			file_handle
		);

		LARGE_INTEGER file_size;
		if (GetFileSizeEx(file_handle.handle, &file_size) == false) {
			__debugbreak();
		}

		return file_size.QuadPart;
	}

	template<typename T>
	std::chrono::time_point<std::chrono::system_clock> File<T>::lastModifiedTime(string file_path)
	{
		win32::Handle file_handle;
		createFile(file_path,
			GENERIC_READ,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			file_handle
		);

		FILETIME filetime;
		win32::check(GetFileTime(file_handle.handle, nullptr, nullptr, &filetime));

		uint64_t ticks;
		ticks = filetime.dwLowDateTime;
		ticks |= (uint64_t)filetime.dwHighDateTime << 32;

		// TODO: this os not corect windows time is different
		return time_point(std::chrono::milliseconds(ticks));
	}
}
