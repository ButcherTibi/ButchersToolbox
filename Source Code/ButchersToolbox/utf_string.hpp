#pragma once

// Standard
#include <string>
#include <vector>


// Forward
class utf8string;
class utf8string_iter;


// NOTE:
// Upon analizing the memory cost of rendering a single character on the GPU
// The idea of using UTF-8 to save memory of rendered characters is absolutely
// hilarious and absurd (pretty easy to make a utf-8 string type)
// it takes around 120 bytes of memory of CPU and GPU to render a single character


/* Low level Utilities */

// looks at the byte and determines how many bytes are used for the character
// in the encoding
// returns 0 if invalid
uint32_t getUTF8_SequenceLength(uint8_t first_byte);

// get how many bytes are required to encode the code point
// returns 0 if invalid
uint32_t getUTF8_CodePointLength(uint32_t code_point);

// encode code point into bytes
// ensure you call the right one based on code point
void encodeUTF8_CodePoint(uint32_t code_point,
	uint8_t& byte_0);

void encodeUTF8_CodePoint(uint32_t code_point,
	uint8_t& byte_0, uint8_t& byte_1);

void encodeUTF8_CodePoint(uint32_t code_point,
	uint8_t& byte_0, uint8_t& byte_1, uint8_t& byte_2);

void encodeUTF8_CodePoint(uint32_t code_point,
	uint8_t& byte_0, uint8_t& byte_1, uint8_t& byte_2, uint8_t& byte_3);

// get the number of bytes between iterators
uint32_t byteCount(utf8string_iter& begin, utf8string_iter& end);


class utf8string_iter {
public:
	utf8string* parent = nullptr;
	int32_t byte_index;

public:
	/* Iterator interface */

	void operator++();
	void operator++(int);
	void operator--();
	void operator--(int);


	/* In place iteration */

	void prev(uint32_t count = 1);
	void next(uint32_t count = 1);

	/// <summary>
	/// Calculate at which caracter does this iterator point to
	/// </summary>
	int32_t characterIndex();

	/// <summary>
	/// Retrieve/decode unicode code point at curent index
	/// </summary>
	uint32_t codePoint();

	/// <summary>
	/// Get character as utf8string
	/// </summary>
	utf8string get();

	/// <summary>
	/// Get pointer to the bytes in utf8string parent
	/// </summary>
	uint8_t* data();

	bool isBeforeBegin();
	bool isNotAtNull();
	bool isAtNull();

	/// <summary>
	/// Swap the contents of this iterator with other
	/// </summary>
	/// <param name="other">Must be iterator to same parent string</param>
	void swap(utf8string_iter& other);
};

bool operator==(utf8string_iter a, utf8string_iter b);
bool operator!=(utf8string_iter a, utf8string_iter b);
bool operator<(utf8string_iter a, utf8string_iter b);
bool operator>(utf8string_iter a, utf8string_iter b);
bool operator<=(utf8string_iter a, utf8string_iter b);
bool operator>=(utf8string_iter a, utf8string_iter b);

bool operator==(utf8string_iter a, const char8_t* utf8_string_literal);


class utf8string {
public:
	using iter = utf8string_iter;
public:
	std::vector<uint8_t> bytes;

public:
	utf8string();
	utf8string(const char8_t* utf8_string_literal);
	utf8string(std::string& string);
	utf8string(std::vector<uint8_t>& bytes);


	/* Queries */

	/// <summary>
	/// Return the number of code points (does not include null character)
	/// <para>NOTE: to calculate the length the entire string must be traversed</para>
	/// </summary>
	uint32_t length();

	// faster than calling length and checking for 0

	/// <summary>
	/// Checks whether or not the string contains characters
	/// </summary>
	bool isEmpty();


	/* Iterators */

	/// <summary>
	/// Creates an iterator that points to before the begining of the string.
	/// </summary>
	/// <returns>Return iterator with byte index of -1 before begin iter</returns>
	iter before();

	/// <summary>
	/// Creates an iterator that points to the begining of the string.
	/// </summary>
	/// <returns>Iterator with byte index of 0</returns>
	iter begin();

	/// <summary>
	/// Creates an iterator that points to the first character of the string.
	/// <para>Nicer name for begin().</para>
	/// </summary>
	/// <returns>Iterator with byte index of 0</returns>
	iter first();

	/// <summary>
	/// Creates an iterator to the last character, excludes null character
	/// </summary>
	iter last();
	
	/// <summary>
	/// Creates an iterator that points after the last character
	/// </summary>
	/// <returns>Iterator that points at null character</returns>
	iter after();

	/// <summary>
	/// Creates an iterator that points after the last character
	/// <para>Nicer name for after().</para>
	/// </summary>
	/// <returns>Iterator that points at null character</returns>
	iter end();


	/* Insert */

	/// <summary>
	/// Encodes de code point into one or more bytes that will be appended to string
	/// </summary>
	/// <param name="code_point"> = the code point of the character, NOT the encoded value</param>
	void push(uint32_t code_point);

	/// <summary>
	/// Adds the new content at the end of the string
	/// </summary>
	void push(utf8string& new_content);

	/// <summary>
	/// Adds the new content at the end of the string
	/// </summary>
	void push(const char8_t* utf8_string_literal);

	/// <summary>
	/// Adds the new content before the start of the string
	/// </summary>
	void prepend(utf8string& new_content);

	/// <summary>
	/// Adds the new content before the start of the string
	/// </summary>
	void prepend(const char8_t* utf8_string_literal);

	/// <summary>
	/// Inserts characters from new content, past the character pointed by location
	/// </summary>
	void insertAfter(iter& location, utf8string& new_content);

	/// <summary>
	/// Inserts characters from between new_content_start and new_content_end, past the character pointed by location.
	/// <para>new_content_start and new_content_end must belong the same string</para>
	/// </summary>
	void insertAfter(iter& location,
		iter& new_content_start, iter& new_content_end);

	/// <summary>
	/// Overwrites existing characters replacing them with new_content.
	/// <para>Original string is resized to fit new_conent if required</para>
	/// </summary>
	void overwrite(iter& location, utf8string& new_content);

	/// <summary>
	/// Overwrites existing characters replacing them with new_content.
	/// <para>Original string is resized to fit new_conent if required</para>
	/// </summary>
	void overwrite(iter& location,
		iter& new_content_start, uint32_t new_content_length);

	/// <summary>
	/// Overwrites existing characters replacing them with new_content.
	/// <para>Original string is resized to fit new_conent if required</para>
	/// </summary>
	void overwrite(iter& location,
		iter& new_content_start, iter& new_content_end);

	/// <summary>
	/// Deletes selection and inserts new content.
	/// <para>Designed for use in text edit. Method is faster than delete + insert.</para>
	/// </summary>
	void replaceSelection(iter& selection_start, uint32_t selection_length,
		utf8string& new_content);


	/* Delete */

	/// <summary>
	/// Delete a number of characters from the end
	/// </summary>
	void pop(uint32_t chars_to_delete = 1);

	/// <summary>
	/// Delete a number of characters from the start of the string
	/// </summary>
	void popFront(uint32_t chars_to_delete = 1);
	
	// erase starting at location
	void erase(iter& selection_start, uint32_t selection_length);

	void erase(iter& selection_start, iter& selection_end);


	/* Output */

	// extract selection into a new utf8string
	utf8string extract(iter& start, iter& end);
	utf8string extract(iter& start, uint32_t length);

	// fill other string with bytes content (excludes \0)
	void fill(std::string& other);

	// returns a C style null terminated string
	const char* c_str();
};

bool operator==(utf8string& a, const char8_t* utf8_string_literal);
bool operator!=(utf8string& a, const char8_t* utf8_string_literal);
