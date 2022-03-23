// Header
#include "JSON.hpp"

using namespace std::string_literals;
using namespace json;


char Importer::getChar()
{
	return (*file)[pos.i];
}

void Importer::advance()
{
	char8_t chara = (*file)[pos.i];

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

bool Importer::checkKeyword(std::string keyword)
{
	FilePosition start = pos;
	uint32_t keyword_index = 0;

	while (pos.i < file->size()) {

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

	while (pos.i < file->size()) {

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

	while (pos.i < file->size()) {

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
	char unexpected_symbol = (*file)[unexpected_pos.i];

	Error& new_err = errors.emplace_back();
	new_err.line = pos.line;
	new_err.column = pos.column;
	new_err.msg = "Unexpected symbol '"s + unexpected_symbol + "' "s + msg;
}

bool Importer::parseFieldName(std::string& r_field_name)
{
	if (skipToSymbol('"')) {

		advance();

		while (pos.i < file->size()) {

			char8_t chara = getChar();

			if ('\\') {
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

		FilePosition start = pos;
		advance();

		// object has no fields
		if (skipToSymbol('}')) {

			uint32_t result = values.size();
			values.emplace_back().emplace<std::vector<Field>>();
			return result;
		}

		_obj.clear();

		while (pos.i < file->size()) {

			Field& new_field = _obj.emplace_back();

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

				advance();

				uint32_t result = values.size();
				auto& value = values.emplace_back().emplace<std::vector<Field>>();
				value = _obj;
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

		FilePosition start = pos;
		advance();

		// empty array
		if (skipToSymbol(']')) {

			uint32_t result = values.size();
			values.emplace_back().emplace<std::vector<uint32_t>>();
			return result;
		}
		else {
			_array.clear();

			while (pos.i < file->size()) {

				uint32_t value_idx = parseValue();
				if (value_idx != invalid_index) {
					_array.push_back(value_idx);
				}
				else {
					return invalid_index;
				}

				// array has another value
				if (skipToSymbol(',')) {
					advance();
				}
				else if (skipToSymbol(']')) {

					uint32_t result = values.size();
					auto& value = values.emplace_back().emplace<std::vector<uint32_t>>();
					value = _array;

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
		number_str.clear();

		advance();

		while (pos.i < file->size()) {

			char8_t chara = getChar();

			if (chara == '\\') {

				advance();

				switch (getChar()) {
				case '\"': {
					number_str.push_back('\"');
					break;
				}
				case '\\': {
					number_str.push_back('\\');
					break;
				}
				case 'n': {
					number_str.push_back('\n');
				}
				case 't': {
					number_str.push_back('\t');
				}
				default:
					number_str.push_back((*file)[pos.i - 1]);
					number_str.push_back(getChar());
				}
			}
			else if (chara == '"') {

				uint32_t result = values.size();
				auto& value = values.emplace_back().emplace<std::string>();
				value = number_str;

				return result;
			}
			else {
				number_str.push_back(chara);
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
		number_str.clear();

		while (pos.i < file->size()) {

			char8_t chara = getChar();

			if (chara == '+' || chara == '-' ||
				('0' <= chara && chara <= '9') ||
				chara == '.' || chara == 'e' || chara == 'E')
			{
				number_str.push_back(chara);
				advance();
			}
			else {
				try {
					uint32_t result = values.size();
					double& value = values.emplace_back().emplace<double>();
					value = std::stold(number_str);

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

		if (checkKeyword("true")) {

			uint32_t result = values.size();
			auto& value = values.emplace_back().emplace<bool>();
			value = true;

			advance();
			return result;
		}
		else {
			logError_unexpectedSymbol("while checking for 'true' keyword in boolean");
			return invalid_index;
		}
	}
	// boolean false
	else if (chara == 'f') {

		if (checkKeyword("false")) {

			uint32_t result = values.size();
			auto& value = values.emplace_back().emplace<bool>();
			value = false;

			advance();
			return result;
		}
		else {
			logError_unexpectedSymbol("while checking for 'false' keyword in boolean");
			return invalid_index;
		}
	}
	// null
	else if (chara == 'n') {

		if (checkKeyword("null")) {

			uint32_t result = values.size();
			values.emplace_back().emplace<nullptr_t>();

			advance();
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

bool Importer::parseFile(std::vector<char8_t>& text)
{
	if (parseValue() != invalid_index) {
		return true;
	}
	else {
		return false;
	}
}
