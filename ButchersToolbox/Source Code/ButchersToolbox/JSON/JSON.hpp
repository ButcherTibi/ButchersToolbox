#pragma once

// Standard
#include <string>
#include <variant>
#include <vector>


namespace json {

	const uint32_t invalid_index = 0xFFFF'FFFF;

	typedef std::vector<uint32_t> Array;

	struct Field {
		std::string name;
		uint32_t value;
	};

	typedef std::vector<Field> Object;

	typedef std::variant<
		bool,  // boolean
		double,  // Number
		std::string,  // String
		Array,  // Array
		Object,  // Object
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

	class Structure {
	public:
		std::vector<Value> values;
		
		FilePosition pos;
		FilePosition unexpected_pos;

		std::vector<char8_t>* text;
		std::vector<Error> errors;  // lookup here for errors

		// Settings
		bool pretty;

		// Mem cache
		std::string _number;

	private:
		char8_t getChar();

		void advance();

		bool isAtWhiteSpace();

		bool skipKeyword(std::string keyword);


		/* Skip functions */

		bool skipSpacing();

		bool skipToSymbol(char8_t symbol);


		/* Error functions */

		void logError(std::string msg);

		void logError_unexpectedSymbol(std::string msg);


		/* Parse functions */

		bool parseFieldName(std::string& r_field_name);

		uint32_t parseValue();

		
		/* Write functions */

		void writeString(std::string str);

		void writeSpace();

		void writeNewLine();

		void writeIndentation(uint32_t indentation);

		void writeValue(uint32_t value_idx, uint32_t indentation, bool field_value);


	public:
		/* Read */

		/// <summary>
		/// Gets the top level value of the JSON structure
		/// </summary>
		Value& getRoot();

		template<typename T>
		T& get(Value& value);

		/// <summary>
		/// Get field value in JSON object base on field name
		/// </summary>
		/// <param name="object">= Object where the field is</param>
		/// <param name="field_name">= Name of the field to find</param>
		/// <returns>Reference to found field value</returns>
		Value& getValue(Object& object, std::string field_name);

		/// <summary>
		/// Get field value in JSON object base on field index
		/// </summary>
		/// <param name="object">= Object where the field is</param>
		/// <param name="field_index">= Index of the field in object</param>
		/// <returns>Reference to found field value</returns>
		Value& getValue(Object& object, uint32_t field_index);


		/* Write */

		/// <summary>
		/// Add a new value to the JSON array
		/// </summary>
		/// <param name="array">= Array to add value to</param>
		/// <returns>Reference to new element in array</returns>
		template<typename T>
		T& add(Array& array);

		/// <summary>
		/// Add a new field to JSON object
		/// </summary>
		/// <param name="object">= Object to add field to</param>
		/// <param name="field_name">= Name of the new field to be added</param>
		/// <returns>Reference to new field value in object</returns>
		template<typename T>
		T& addField(Object& object, std::string field_name);


		/* API */
	
		/// <summary>
		/// Parses the JSON and creates a structure of values
		/// </summary>
		/// <param name="json_text">= JSON to be parsed into values</param>
		/// <returns>False if the were parsing errors, errors are found the errors field</returns>
		bool parse(std::vector<char8_t>& json_text);

		/// <summary>
		/// Creates a JSON representation from the structure of values
		/// </summary>
		/// <param name="r_json_text">= The resulting JSON</param>
		/// <param name="pretty">= Enables pretty indentation of the resulting JSON</param>
		void toString(std::vector<char8_t>& r_json_text, bool pretty = false);
	};
}
