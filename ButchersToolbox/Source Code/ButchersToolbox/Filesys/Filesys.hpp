#pragma once

// Standard
#include <cstdint>
#include <string>
#include <vector>


namespace filesys {

	void getExecutablePath(std::string& exe_path);

	void readFile(std::string& path, std::vector<uint8_t>& r_bytes);


	class Path {
	public:
		std::vector<std::string> entries;

	public:
		void _pushPathToEntries(std::string& path);

		Path() = default;
		Path(std::string path);

		void append(std::string entry);
		void pop(uint32_t count = 1);

		std::string toString();
	};
}
