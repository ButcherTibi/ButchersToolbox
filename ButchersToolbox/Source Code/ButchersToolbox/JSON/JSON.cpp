// Header
#include "JSON.hpp"

using namespace std::string_literals;
using namespace json;


char8_t Importer::getChar()
{
	return (*text)[pos.i];
}

void Importer::advance()
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

bool Importer::isAtWhiteSpace()
{
	char8_t chara = getChar();
	return chara == ' ' || chara == '\t' ||
		chara == '\n' || chara == '\r';
}

bool Importer::skipKeyword(std::string keyword)
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

bool Importer::skipSpacing()
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

bool Importer::skipToSymbol(char8_t symbol)
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

void Importer::logError(std::string msg)
{
	Error& new_err = errors.emplace_back();
	new_err.msg = msg;
	new_err.line = pos.line;
	new_err.column = pos.column;
}

void Importer::logError_unexpectedSymbol(std::string msg)
{
	char unexpected_symbol = (*text)[unexpected_pos.i];

	Error& new_err = errors.emplace_back();
	new_err.line = pos.line;
	new_err.column = pos.column;
	new_err.msg = "Unexpected symbol '"s + unexpected_symbol + "' "s + msg;
}

bool Importer::parseFieldName(std::string& r_field_name)
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

//enum class NumberParseMode {
//	INTEGER,
//	FRACTION,
//	EXPONENT,
//};
//
//#define isUTF8Number(c) \
//	c > 0x2f && c < 0x3a
//
//void convertCharsToInt(std::vector<char>& integer_chars, int64_t& int_num)
//{
//	uint64_t m = 1;
//
//	int32_t count = (int32_t)integer_chars.size() - 1;
//	for (int32_t idx = count; idx != -1; idx--) {
//
//		int64_t digit = (int64_t)(integer_chars[idx] - '0');
//		int64_t a = digit * m;
//
//		int_num += a;
//		m *= 10;
//	}
//}
//
//ErrStack Graph::_parseNumber(uint64_t& i, std::vector<uint8_t>& text, Value& number)
//{
//	this->integer_chars.clear();
//	this->frac_chars.clear();
//
//	NumberParseMode mode = NumberParseMode::INTEGER;
//
//	int64_t int_sign = 1;
//	int64_t int_part = 0;
//	bool exp_sign = 1;
//
//	for (; i < text.size(); i++) {
//
//		uint8_t& c = text[i];
//
//		switch (mode) {
//		case NumberParseMode::INTEGER:
//
//			if (c == '+') {
//				int_sign = 1;
//			}
//			else if (c == '-') {
//				int_sign = -1;
//			}
//			else if (isUTF8Number(c)) {
//				integer_chars.push_back(c);
//			}
//			else {
//				convertCharsToInt(integer_chars, int_part);
//				int_part *= int_sign;
//
//				if (c == '.') {
//					mode = NumberParseMode::FRACTION;
//				}
//				else {
//					number.value = (double)int_part;
//					return ErrStack();
//				}
//			}
//			break;
//
//		case NumberParseMode::FRACTION:
//
//			if (isUTF8Number(c)) {
//				frac_chars.push_back(c);
//			}
//			else if (c == 'e' || c == 'E') {
//				mode = NumberParseMode::EXPONENT;
//			}
//			else {
//				// Extract 0.1234
//				int64_t frac_part;
//				convertCharsToInt(frac_chars, frac_part);
//				double frac_part_dbl = (double)frac_part / std::pow(10.0, (double)frac_chars.size());
//
//				number.value = (double)int_part + frac_part_dbl;
//				return ErrStack();
//			}
//			break;
//
//		case NumberParseMode::EXPONENT: {
//
//			if (c == '+') {
//				exp_sign = 1;
//			}
//			else if (c == '-') {
//				exp_sign = 0;
//			}
//			else if (isUTF8Number(c)) {
//
//			}
//			else {
//				number.value = (double)1.0;
//				return ErrStack();
//			}
//			break;
//		}
//		}
//	}
//	return ErrStack(code_location,
//		msgWithPos("premature end of characters in file when parsing number",
//			i - 1, text));
//}

uint32_t Importer::parseValue()
{
	if (skipSpacing() == false) {
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

			if (chara == '\\') {

				advance();

				switch (getChar()) {
				case '\"': {
					_number.push_back('\"');
					break;
				}
				case '\\': {
					_number.push_back('\\');
					break;
				}
				case 'n': {
					_number.push_back('\n');
				}
				case 't': {
					_number.push_back('\t');
				}
				default:
					_number.push_back((*text)[pos.i - 1]);
					_number.push_back(getChar());
				}
			}
			else if (chara == '"') {

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

bool Importer::parseText(std::vector<char8_t>& new_text)
{
	text = &new_text;
	pos.line = 1;
	pos.column = 1;

	if (parseValue() != invalid_index) {
		return true;
	}
	else {
		return false;
	}
}

void Importer::writeString(std::string str)
{
	for (char chara : str) {
		text->push_back(chara);
	}
}

void Importer::writeSpace()
{
	if (pretty) {
		text->push_back(' ');
	}
}

void Importer::writeNewLine()
{
	if (pretty) {
		text->push_back('\n');
	}
}

void Importer::writeIndentation(uint32_t indentation)
{
	if (pretty) {
		for (uint32_t i = 0; i < indentation; i++) {
			text->push_back('\t');
		}
	}
}

void Importer::writeValue(uint32_t value_idx, uint32_t indentation, bool field_value)
{
	Value& value = values[value_idx];

	// Object
	if (std::holds_alternative<std::vector<Field>>(value)) {

		auto& obj = std::get<std::vector<Field>>(value);

		text->push_back('{');
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

		if (field_value) {
			writeIndentation(indentation);
		}
		else {
			writeIndentation(indentation + 1);
		}
		writeString("}");
	}
	// Array
	else if (std::holds_alternative<std::vector<uint32_t>>(value)) {

		auto& arr = std::get<std::vector<uint32_t>>(value);

		text->push_back('[');
		writeNewLine();

		for (uint32_t i = 0; i < arr.size(); i++) {

			uint32_t array_value_idx = arr[i];

			writeIndentation(indentation + 1);

			writeValue(array_value_idx, indentation, false);

			// Next value
			if (i < arr.size() - 1) {
				text->push_back(',');
				writeNewLine();
			}
		}

		writeNewLine();

		if (field_value) {
			writeIndentation(indentation);
		}
		else {
			writeIndentation(indentation + 1);
		}
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

void Importer::writeText(std::vector<char8_t>& new_text, bool new_pretty)
{
	text = &new_text;
	pos.i = 0;
	pos.line = 1;
	pos.column = 1;

	// Settings
	this->pretty = new_pretty;

	writeValue(0, 0, true);
}
