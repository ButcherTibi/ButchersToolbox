// Header
#include "JSON.hpp"

// Mine
#include "ButchersToolbox/Filesys/Filesys.hpp"
#include "ButchersToolbox/JSON/JSON.hpp"


void JSON_tests::performance()
{
	filesys::Path read_path;
	filesys::Path write_path;
	{
		auto dir_path = filesys::Path::executablePath();
		dir_path.pop(3);

		read_path = dir_path;
		write_path = dir_path;

		read_path.append("/TestingClient/Tests/big_parse_test.json");
		write_path.append("/TestingClient/Tests/big_toString_test.json");
	}

	std::vector<char8_t> readed_text;
	read_path.readFile(readed_text);

	json::Structure structure;
	structure.parse(readed_text);

	if (structure.errors.size()) {
		printf(structure.errors[0].msg.c_str());
	}
	else {
		std::vector<char8_t> writen_text;
		structure.toString(writen_text, true);

		write_path.writeFile(writen_text);
	}
}

void JSON_tests::correctness()
{
	filesys::Path read_path;
	filesys::Path write_path;
	{
		auto dir_path = filesys::Path::executablePath();
		dir_path.pop(3);

		read_path = dir_path;
		write_path = dir_path;

		read_path.append("/TestingClient/Tests/read_test.json");
		write_path.append("/TestingClient/Tests/write_test.json");
	}

	std::vector<char8_t> readed_text;
	read_path.readFile(readed_text);

	json::Structure structure;
	structure.parse(readed_text);

	if (structure.errors.size()) {
		printf(structure.errors[0].msg.c_str());
	}
	else {
		// Read
		json::Value& root_val = structure.getRoot();

		auto& obj = structure.get<json::Object>(root_val);

		json::Value& field_1_val = structure.getValue(obj, "field_1");
		double& field_1 = structure.get<double>(field_1_val);

		json::Value& field_2_val = structure.getValue(obj, 1);
		json::Object& field_2 = structure.get<json::Object>(field_2_val);

		json::Value& field_3_val = structure.getValue(obj, 2);
		json::Array& field_3 = structure.get<json::Array>(field_3_val);

		// Write
		structure.add<nullptr_t>(field_3);

		double& new_field = structure.addField<double>(obj, "new_field");
		new_field = 1234;
	}

	std::vector<char8_t> writen_text;
	structure.toString(writen_text, true);

	write_path.writeFile(writen_text);
}
