// Header
#include "JSON.hpp"

using namespace std::string_literals;
using namespace json;


char8_t Structure::getChar()
{
	return (*text)[pos.i];
}

void Structure::advance()
{
	char8_t chara = (*text)[pos.i];

	if (chara == '\n') {
		pos.column = 0;
		pos.line++;
	}
	else {
		pos.column++;
	}

	pos.i += 1;
}

bool Structure::isAtWhiteSpace()
{
	char8_t chara = getChar();
	return chara == ' ' || chara == '\t' ||
		chara == '\n' || chara == '\r';
}

bool Structure::skipKeyword(std::string keyword)
{
	FilePosition start = pos;
	uint32_t keyword_index = 0;

	while (pos.i < text->size()) {

		char8_t chara = getChar();

		if (chara == keyword[keyword_index]) {

			advance();
			keyword_index++;

			if (keyword_index == keyword.size()) {
				return true;
			}
		}
		else {
			pos = start;
			return false;
		}
	}

	pos = start;
	return false;
}

bool Structure::skipSpacing()
{
	FilePosition start = pos;

	while (pos.i < text->size()) {

		char8_t chara = getChar();

		if (isAtWhiteSpace() == false) {
			return true;
		}
		else {
			advance();
		}
	}

	pos = start;
	return false;
}

bool Structure::skipToSymbol(char8_t symbol)
{
	FilePosition start = pos;

	while (pos.i < text->size()) {

		char8_t chara = getChar();
		
		if (isAtWhiteSpace() == false) {

			if (chara == symbol) {
				return true;
			}
			else {
				unexpected_pos = pos;
				pos = start;
				return false;
			}
		}
		else {
			advance();
		}
	}

	pos = start;
	return false;
}

void Structure::logError(std::string msg)
{
	Error& new_err = errors.emplace_back();
	new_err.msg = msg;
	new_err.line = pos.line;
	new_err.column = pos.column;
}

void Structure::logError_unexpectedSymbol(std::string msg)
{
	char unexpected_symbol = (*text)[unexpected_pos.i];

	Error& new_err = errors.emplace_back();
	new_err.line = pos.line;
	new_err.column = pos.column;
	new_err.msg = "Unexpected symbol '"s + unexpected_symbol + "' "s + msg;
}

bool Structure::parseFieldName(std::string& r_field_name)
{
	if (skipToSymbol('"')) {

		advance();

		while (pos.i < text->size()) {

			char8_t chara = getChar();

			if (chara == '\\') {
				logError("Escape sequences are not allowed in object field names");
				return false;
			}
			else if (chara == '"') {

				if (r_field_name.empty()) {
					logError("Object field name is empty");
					return false;
				}
				else {
					advance();
					return true;
				}
			}
			else {
				r_field_name.push_back(chara);
				advance();
			}
		}

		Error& new_err = errors.emplace_back();
		new_err.msg = "Could not find closing '\"' in object field name";
		return false;
	}
	else {
		logError_unexpectedSymbol(
			"while looking for '\"' in object field name"
		);
		return false;
	}
}

uint32_t Structure::parseValue()
{
	if (skipSpacing() == false) {
		logError("Found nothing but spacing");
		return invalid_index;
	}

	char8_t chara = getChar();

	// object
	if (chara == '{') {

		uint32_t result = values.size();
		values.emplace_back().emplace<std::vector<Field>>();

		FilePosition start = pos;
		advance();

		// object has no fields
		if (skipToSymbol('}')) {
			advance();
			return result;
		}

		std::vector<Field> obj;

		while (pos.i < text->size()) {

			Field& new_field = obj.emplace_back();

			// Field name
			if (parseFieldName(new_field.name) == false) {
				return invalid_index;
			}

			// Separator
			if (skipToSymbol(':')) {
				advance();
			}
			else {
				logError_unexpectedSymbol("while looking for ':' in object field");
				return invalid_index;
			}

			// Field value
			new_field.value = parseValue();
			if (new_field.value == invalid_index) {
				return invalid_index;
			}

			// Next field or end of object
			if (skipToSymbol(',')) {
				advance();
			}
			else if (skipToSymbol('}')) {

				auto& value = std::get<std::vector<Field>>(values[result]);
				value = obj;

				advance();
				return result;
			}
			else {
				logError_unexpectedSymbol("while looking for ',' or '}' in object");
				return invalid_index;
			}
		}

		pos = start;
		logError("Could not find closing '}' in object");
		return invalid_index;
	}
	// array
	else if (chara == '[') {

		uint32_t result = values.size();
		values.emplace_back().emplace<std::vector<uint32_t>>();

		FilePosition start = pos;
		advance();

		// empty array
		if (skipToSymbol(']')) {
			advance();
			return result;
		}
		else {
			std::vector<uint32_t> array;

			while (pos.i < text->size()) {

				uint32_t value_idx = parseValue();
				if (value_idx != invalid_index) {
					array.push_back(value_idx);
				}
				else {
					return invalid_index;
				}

				// array has another value
				if (skipToSymbol(',')) {
					advance();
				}
				else if (skipToSymbol(']')) {

					advance();

					auto& value = std::get<std::vector<uint32_t>>(values[result]);
					value = array;

					return result;
				}
				else {
					logError_unexpectedSymbol("while looking for ',' or ']' in array");
					return invalid_index;
				}
			}
			
			pos = start;
			logError("Could not find closing ']' in array");
			return invalid_index;
		}
	}
	// string
	else if (chara == '\"') {

		FilePosition start = pos;
		_number.clear();

		advance();

		while (pos.i < text->size()) {

			char8_t chara = getChar();

			if (chara == '"') {

				uint32_t result = values.size();
				auto& value = values.emplace_back().emplace<std::string>();
				value = _number;

				advance();
				return result;
			}
			else {
				_number.push_back(chara);
			}

			advance();
		}

		pos = start;
		logError("Could not find closing '\"' in string");
		return invalid_index;
	}
	// number
	else if (chara == '+' || chara == '-' ||
		('0' <= chara && chara <= '9'))
	{
		FilePosition start = pos;
		_number.clear();

		while (pos.i < text->size()) {

			char8_t chara = getChar();

			if (chara == '+' || chara == '-' ||
				('0' <= chara && chara <= '9') ||
				chara == '.' || chara == 'e' || chara == 'E')
			{
				_number.push_back(chara);
				advance();
			}
			else {
				try {
					uint32_t result = values.size();
					double& value = values.emplace_back().emplace<double>();
					value = std::stold(_number);

					return result;
				}
				catch (std::exception) {
					logError("Could not parse number");
					return invalid_index;
				}
			}
		}

		pos = start;
		logError("Unexpected end of file while looking for number");
		return invalid_index;
	}
	// boolean true
	else if (chara == 't') {

		if (skipKeyword("true")) {

			uint32_t result = values.size();
			auto& value = values.emplace_back().emplace<bool>();
			value = true;

			return result;
		}
		else {
			logError_unexpectedSymbol("while checking for 'true' keyword in boolean");
			return invalid_index;
		}
	}
	// boolean false
	else if (chara == 'f') {

		if (skipKeyword("false")) {

			uint32_t result = values.size();
			auto& value = values.emplace_back().emplace<bool>();
			value = false;

			return result;
		}
		else {
			logError_unexpectedSymbol("while checking for 'false' keyword in boolean");
			return invalid_index;
		}
	}
	// null
	else if (chara == 'n') {

		if (skipKeyword("null")) {

			uint32_t result = values.size();
			values.emplace_back().emplace<nullptr_t>();

			return result;
		}
		else {
			logError_unexpectedSymbol("while checking for 'null' keyword");
			return invalid_index;
		}
	}
	else {
		logError("Unexpected character while looking for value");
		return invalid_index;
	}
}

bool Structure::parse(std::vector<char8_t>& new_text)
{
	text = &new_text;
	pos.i = 0;
	pos.line = 1;
	pos.column = 1;

	if (parseValue() != invalid_index) {
		return true;
	}
	else {
		return false;
	}
}

void Structure::writeString(std::string str)
{
	for (char chara : str) {
		text->push_back(chara);
	}
}

void Structure::writeSpace()
{
	if (pretty) {
		text->push_back(' ');
	}
}

void Structure::writeNewLine()
{
	if (pretty) {
		text->push_back('\n');
	}
}

void Structure::writeIndentation(uint32_t indentation)
{
	if (pretty) {
		for (uint32_t i = 0; i < indentation; i++) {
			text->push_back('\t');
		}
	}
}

void Structure::writeValue(uint32_t value_idx, uint32_t indentation, bool field_value)
{
	Value& value = values[value_idx];

	// Object
	if (std::holds_alternative<std::vector<Field>>(value)) {

		auto& obj = std::get<std::vector<Field>>(value);

		writeString("{");
		writeNewLine();

		for (uint32_t i = 0; i < obj.size(); i++) {

			json::Field& field = obj[i];

			writeIndentation(indentation + 1);

			// Name + Separator
			writeString('\"' + field.name + "\":");
			writeSpace();

			// Value
			writeValue(field.value, indentation + 1, true);

			// Next field
			if (i < obj.size() - 1) {
				text->push_back(',');
				writeNewLine();
			}
		}

		writeNewLine();
		writeIndentation(indentation);
		writeString("}");
	}
	// Array
	else if (std::holds_alternative<std::vector<uint32_t>>(value)) {

		auto& arr = std::get<std::vector<uint32_t>>(value);

		writeString("[");
		writeNewLine();

		for (uint32_t i = 0; i < arr.size(); i++) {

			uint32_t array_value_idx = arr[i];

			writeIndentation(indentation + 1);

			writeValue(array_value_idx, indentation + 1, false);

			// Next value
			if (i < arr.size() - 1) {
				text->push_back(',');
				writeNewLine();
			}
		}

		writeNewLine();
		writeIndentation(indentation);
		writeString("]");
	}
	// string
	else if (std::holds_alternative<std::string>(value)) {

		auto& str = std::get<std::string>(value);
		writeString('\"' + str + '\"');
	}
	// number
	else if (std::holds_alternative<double>(value)) {

		auto& number = std::get<double>(value);

		writeString(std::to_string(number));
	}
	// bool
	else if (std::holds_alternative<bool>(value)) {

		auto& boolean = std::get<bool>(value);

		if (boolean) {
			writeString("true");
		}
		else {
			writeString("false");
		}
	}
	// null
	else if (std::holds_alternative<nullptr_t>(value)) {
		writeString("null");
	}
	else {
		__debugbreak();
	}
}

void Structure::toString(std::vector<char8_t>& new_text, bool new_pretty)
{
	if (values.empty()) {
		return;
	}

	text = &new_text;
	pos.i = 0;
	pos.line = 1;
	pos.column = 1;

	// Settings
	pretty = new_pretty;

	writeValue(0, 0, true);
}

Value& Structure::getRoot()
{
	return values.front();
}

template<typename T>
T& Structure::get(Value& value)
{
	return std::get<T>(value);
}
template bool&        Structure::get<bool>(Value& value);
template double&      Structure::get<double>(Value& value);
template std::string& Structure::get<std::string>(Value& value);
template Array&       Structure::get<Array>(Value& value);
template Object&      Structure::get<Object>(Value& value);
template nullptr_t&   Structure::get<nullptr_t>(Value& value);

Value& Structure::getValue(Object& object, std::string field_name)
{
	for (Field& field : object) {
		if (field.name == field_name) {
			return values[field.value];
		}
	}

	throw "field not found";
}

Value& Structure::getValue(Object& object, uint32_t field_index)
{
	uint32_t field_value = object[field_index].value;
	return values[field_value];
}

template<typename T>
T& Structure::add(Array& array)
{
	array.push_back((uint32_t)values.size());

	return values.emplace_back().emplace<T>();
}
template bool&        Structure::add<bool>(Array& array);
template double&      Structure::add<double>(Array& array);
template std::string& Structure::add<std::string>(Array& array);
template Array&       Structure::add<Array>(Array& array);
template Object&      Structure::add<Object>(Array& array);
template nullptr_t&   Structure::add<nullptr_t>(Array& array);

template<typename T>
T& Structure::addField(Object& object, std::string field_name)
{
	Field& new_field = object.emplace_back();
	new_field.name = field_name;
	new_field.value = (uint32_t)values.size();

	return values.emplace_back().emplace<T>();
}
template bool&        Structure::addField<bool>(Object& object, std::string field_name);
template double&      Structure::addField<double>(Object& object, std::string field_name);
template std::string& Structure::addField<std::string>(Object& object, std::string field_name);
template Array&       Structure::addField<Array>(Object& object, std::string field_name);
template Object&      Structure::addField<Object>(Object& object, std::string field_name);
template nullptr_t&   Structure::addField<nullptr_t>(Object& object, std::string field_name);
