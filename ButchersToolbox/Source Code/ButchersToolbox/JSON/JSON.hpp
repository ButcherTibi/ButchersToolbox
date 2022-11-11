#pragma once

// Standard
#include <string>
#include <variant>
#include <vector>


/**
 * @TODO: add support for comments by modifing the advance function and add a current context field
 * to ignore `//` in string values.
 * @TODO: remove repeated zeroes on number stringify starting from the end to the begining.
*/

namespace json {

	const uint32_t invalid_index = 0xFFFF'FFFF;

	typedef std::vector<uint32_t> Array;

	struct Field {
		/**
		 * @brief Name of the field.
		*/
		std::string name;
		/**
		 * @brief Index to the value in Structure object.
		 * Can be read using the getValue method in Structure.
		*/
		uint32_t value_idx;
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


	bool& getBool(Value& value);
	double& getNumber(Value& value);
	std::string& getString(Value& value);
	Array& getArray(Value& value);
	Object& getObject(Value& value);
	nullptr_t& getNull(Value& value);


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
		std::vector<Value> values;
		
		FilePosition pos;
		FilePosition unexpected_pos;

		std::string* text;
		
		// Settings
		bool pretty;

		// Mem cache
		std::string _number;

	public:
		std::vector<Error> errors;  // lookup here for errors

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

		/**
		 * @brief Get value at index.
		 * @param Index usually from a JSON Array or a field value from a JSON Object.
		*/
		Value& operator[](int);

		/// <summary>
		/// Get field value in JSON object based on field name
		/// </summary>
		/// <param name="object">= Object where the field is</param>
		/// <param name="field_name">= Name of the field to find</param>
		/// <returns>Reference to found field value</returns>
		Value& getValue(Object& object, std::string field_name);

		/// <summary>
		/// Get field value in JSON object based on field index
		/// </summary>
		/// <param name="object">= Object where the field is</param>
		/// <param name="field_index">= Index of the field in object</param>
		/// <returns>Reference to found field value</returns>
		Value& getValue(Object& object, uint32_t field_index);

		/**
		 * @brief Get item value from JSON array at index.
		 * @param index = Index to access JSON array at.
		 * @return Reference to found item.
		*/
		Value& getValue(Array& arr, uint32_t index);


		/* Write */

		/// <summary>
		/// Add a new value to the JSON array
		/// </summary>
		/// <param name="array">= Array to add value to</param>
		/// <returns>Reference to new element in array</returns>
		template<typename T>
		T& addItem(Array& array);

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
		bool parse(std::string& json_text);

		/// <summary>
		/// Creates a JSON representation from the structure of values
		/// </summary>
		/// <param name="r_json_text">= The resulting JSON</param>
		/// <param name="pretty">= Enables pretty indentation of the resulting JSON</param>
		void toString(std::string& r_json_text, bool pretty = false);
	};
}
