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
		auto dir_path = filesys::Path<>::pathToBinary();
		dir_path.pop(3);

		read_path = dir_path;
		write_path = dir_path;

		read_path.append(L"/TestingClient/Tests/JSON/big_parse_test.json");
		write_path.append(L"/TestingClient/Tests/JSON/big_toString_test.json");
	}

	std::string readed_text;
	filesys::File<>::read(read_path.toString(), readed_text);

	json::Structure structure;
	structure.parse(readed_text);

	if (structure.errors.size()) {
		printf(structure.errors[0].msg.c_str());
	}
	else {
		std::string writen_text;
		structure.toString(writen_text, true);

		filesys::File<>::write(write_path.toString(), writen_text.size(), writen_text.data());
	}
}

void JSON_tests::testCorrectness()
{
	filesys::Path read_path;
	filesys::Path write_path;
	{
		auto dir_path = filesys::Path<>::pathToBinary();
		dir_path.pop(3);

		read_path = dir_path;
		write_path = dir_path;

		read_path.append(L"/TestingClient/Tests/JSON/read_test.json");
		write_path.append(L"/TestingClient/Tests/JSON/write_test.json");
	}

	std::string readed_text;
	filesys::File<>::read(read_path.toString(), readed_text);

	json::Structure structure;
	structure.parse(readed_text);

	if (structure.errors.size()) {
		printf(structure.errors[0].msg.c_str());
	}
	else {
		// Read
		json::Value& root_val = structure.getRoot();

		auto& obj = json::getObject(root_val);

		json::Value& field_1_val = structure.getValue(obj, "field_1");
		double& field_1 = json::getNumber(field_1_val);

		json::Value& field_2_val = structure.getValue(obj, 1);
		json::Object& field_2 = json::getObject(field_2_val);

		json::Value& field_3_val = structure.getValue(obj, 2);
		json::Array& field_3 = json::getArray(field_3_val);

		// Write
		structure.addItem<nullptr_t>(field_3);

		double& new_field = structure.addField<double>(obj, "new_field");
		new_field = 1234;
	}

	std::string writen_text;
	structure.toString(writen_text, true);

	filesys::File<>::write(write_path.toString(), writen_text.size(), writen_text.data());
}
