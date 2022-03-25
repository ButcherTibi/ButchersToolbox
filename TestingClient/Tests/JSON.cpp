// Header
#include "JSON.hpp"

// Mine
#include "ButchersToolbox/Filesys/Filesys.hpp"
#include "ButchersToolbox/JSON/JSON.hpp"


void JSON_tests::test()
{
	std::string read_path;
	std::string write_path;
	{
		std::string exe_path;
		filesys::getExecutablePath(exe_path);

		filesys::Path dir_path(exe_path);
		dir_path.pop(3);

		filesys::Path path = dir_path;
		path.append("/TestingClient/Tests/read_test.json");
		read_path = path.toString();

		path = dir_path;
		path.append("/TestingClient/Tests/write_test.json");
		write_path = path.toString();
	}

	std::vector<char8_t> readed_text;
	filesys::readFile(read_path, readed_text);

	json::Importer serializer;
	serializer.parseText(readed_text);

	if (serializer.errors.size()) {
		printf(serializer.errors[0].msg.c_str());
	}

	std::vector<char8_t> writen_text;
	serializer.writeText(writen_text, true);

	filesys::writeFile(write_path, writen_text);
}
