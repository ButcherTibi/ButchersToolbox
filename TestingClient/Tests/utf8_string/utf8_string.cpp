#include "utf8_string.hpp"

#include <array>


namespace utf8_string_tests {

	void testInsertAfter()
	{
		// Before
		{
			utf8_string str = u8"ăîșț";
			utf8_string_iter str_iter = str.before();
			utf8_string new_content = u8"Ă";

			utf8_string new_str = str;
			new_str.insert(str_iter, new_content);

			if (new_str != u8"Ăăîșț") {
				throw std::exception();
			}
		}

		// Begin
		{
			utf8_string str = u8"ăîșț";
			utf8_string_iter str_iter = str.begin();
			utf8_string new_content = u8"Ă";

			utf8_string new_str = str;
			new_str.insert(str_iter, new_content);

			if (new_str != u8"ăĂîșț") {
				throw std::exception();
			}
		}

		// Last
		{
			utf8_string str = u8"ăîșț";
			utf8_string_iter str_iter = str.last();
			utf8_string new_content = u8"Ă";

			utf8_string new_str = str;
			new_str.insert(str_iter, new_content);

			if (new_str != u8"ăîșțĂ") {
				throw std::exception();
			}
		}
	}

	void testPushAndPrepend()
	{
		// Append
		{
			utf8_string str = u8"Cârnați";
			str.push(u8" cu mămăliguță");

			if (str != u8"Cârnați cu mămăliguță") {
				throw std::exception();
			}
		}

		// Prepend
		{
			utf8_string str = u8" cu mămăliguță";
			str.prepend(u8"Cârnați");

			if (str != u8"Cârnați cu mămăliguță") {
				throw std::exception();
			}
		}
	}

	void testForLoop()
	{
		utf8_string str = u8"ăĂîșț";

		uint32_t index = 0;
		for (auto i = str.begin(); i < str.end(); i++) {

			auto character = i.get();

			switch (index) {
			case 0: {
				if (character != u8"ă") {
					throw std::exception();
				}
				break;
			}
			case 1: {
				if (character != u8"Ă") {
					throw std::exception();
				}
				break;
			}
			case 2: {
				if (character != u8"î") {
					throw std::exception();
				}
				break;
			}
			case 3: {
				if (character != u8"ș") {
					throw std::exception();
				}
				break;
			}
			case 4: {
				if (character != u8"ț") {
					throw std::exception();
				}
				break;
			}
			}

			index++;
		}
	}

	void testErase()
	{
		// Pop
		{
			utf8_string str = u8"ăĂîșț";
			str.pop(3);

			if (str != u8"ăĂ") {
				throw std::exception();
			}
		}

		// Pop Front
		{
			utf8_string str = u8"ăĂîșț";
			str.popFront(3);

			if (str != u8"șț") {
				throw std::exception();
			}
		}

		// Erase Symbols
		{
			utf8_string str = u8"ăĂîșț";

			str.erase(u8"Ăîș");

			if (str != u8"ăț") {
				throw std::exception();
			}
		}
	}

	void testSearch()
	{
		// Contains
		{
			utf8_string str = u8"ăĂîșț";

			if (str.contains(u8"îș") == false ||
				str.contains(u8"Ăîș") == false ||
				str.contains(u8"ăĂîșț") == false)
			{
				throw std::exception();
			}
		}

		// find
		{
			utf8_string str = u8"ăĂîșț";
			auto iter = str.find(u8"îș");

			if (iter.byte_index != 4 || iter.get() != u8"î") {
				throw std::exception();
			}
		}
	}

	void testJoin()
	{
		// Vector
		std::vector<utf8_string> vec = {
			u8"ăĂ", u8"îș", u8"ț"
		};

		utf8_string vec_str = u8"AB";
		vec_str.join(vec, u8"_");

		if (vec_str != u8"AB_ăĂ_îș_ț") {
			throw std::exception();
		}

		// Array
		std::array<utf8_string, 3> arr = {
			u8"ăĂ", u8"îș", u8"ț"
		};

		utf8_string arr_str = u8"AB";
		arr_str.join(vec, u8"_");

		if (arr_str != u8"AB_ăĂ_îș_ț") {
			throw std::exception();
		}
	}

	void testCorrectness()
	{
		testInsertAfter();
		testPushAndPrepend();
		testForLoop();
		testErase();
		testSearch();
		testJoin();
	}
}