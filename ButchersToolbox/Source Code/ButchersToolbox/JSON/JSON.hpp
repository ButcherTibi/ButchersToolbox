#pragma once

// Standard
#include <string>
#include <variant>
#include <vector>


namespace json {

	const uint32_t invalid_index = 0xFFFF'FFFF;

	struct Field {
		std::string name;
		uint32_t value;
	};

	typedef std::variant<
		bool,  // boolean
		double,  // Number
		std::string,  // String
		std::vector<uint32_t>,  // Array
		std::vector<Field>,  // Object
		nullptr_t  // null
	> Value;

	struct Error {
		uint32_t line = 0xFFFF'FFFF;
		uint32_t column = 0xFFFF'FFFF;

		std::string msg;
	};

	struct FilePosition {
		uint32_t i;

		uint32_t line;
		uint32_t column;
	};

	class Importer {
	public:
		std::vector<Value> values;
		
		FilePosition pos;
		FilePosition unexpected_pos;

		std::vector<char8_t>* file;
		std::vector<Error> errors;

		// Mem cache
		std::vector<Field> _obj;
		std::vector<uint32_t> _array;
		std::string number_str;

	public:
		char getChar();

		void advance();

		bool isAtWhiteSpace();

		bool checkKeyword(std::string keyword);


		/* Skip functions */

		bool skipSpacing();

		bool skipToSymbol(char8_t symbol);


		/* Error functions */

		void logError(std::string msg);

		void logError_unexpectedSymbol(std::string msg);


		/* Parse functions */

		bool parseFieldName(std::string& r_field_name);

		uint32_t parseValue();


		/* API */

		bool parseFile(std::vector<char8_t>& file);
	};
}
