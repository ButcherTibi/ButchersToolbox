#pragma once

#include "ButchersToolbox/JSON/JSON.hpp"

namespace JSON_examples {
	void examples() {
		std::string json_txt = R"(
		{
			"object": {
				"string_field": "string value",
				"number_field": 1234.56,
				"bool_field": true,
				"null_field": null
			},
			"array": [
				"elem_0",
				"elem_1",
				"elem_2"
			]
		})";

		json::Structure structure;
		if (structure.parse(json_txt) == false) {
			for (auto& error : structure.errors) {
				printf("line = %d column = %d %s \n",
					error.line, error.column,
					error.msg.c_str()
				);
			}

			throw std::exception("parsing error occurred");
		}

		json::Value& root_val = structure.getRoot();
		json::Object& root = json::getObject(root_val);

		json::Value& object_val = structure.getValue(root, "object");
		json::Object& object = json::getObject(object_val);
		{
			auto& v = structure.getValue(object, "string_field");
			printf("string_field = %s \n", json::getString(v).c_str());

			auto& v1 = structure.getValue(object, "number_field");
			printf("number_field = %f \n", json::getNumber(v1));

			auto& v2 = structure.getValue(object, "bool_field");
			printf("bool_field = %d \n", json::getBool(v2));

			auto& v3 = structure.getValue(object, "null_field");
			printf("null_field = %p \n", (void*)json::getNull(v3));
		}

		json::Value& array_val = structure.getValue(root, "array");
		json::Array& arr = json::getArray(array_val);

		printf("Array values: \n");
		for (auto& value_index : arr) {
			json::Value& value = structure[value_index];
			printf("  %s \n", json::getString(value).c_str());
		}
	}
}