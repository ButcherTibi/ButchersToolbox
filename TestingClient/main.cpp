
// Standard
#include <stdio.h>
#include <iostream>

// Mine
#include <ButchersToolbox\Console.hpp>
#include <ButchersToolbox\utf8_string.hpp>
#include <ButchersToolbox\Arrays.hpp>


namespace utf8string_tests {

	void testInsertAfter()
	{
		// Before
		{
			utf8string str = u8"ăîșț";
			utf8string_iter str_iter = str.before();
			utf8string new_content = u8"Ă";

			utf8string new_str = str;
			new_str.insertAfter(str_iter, new_content);

			if (new_str != u8"Ăăîșț") {
				throw std::exception();
			}
		}

		// Begin
		{
			utf8string str = u8"ăîșț";
			utf8string_iter str_iter = str.begin();
			utf8string new_content = u8"Ă";

			utf8string new_str = str;
			new_str.insertAfter(str_iter, new_content);

			if (new_str != u8"ăĂîșț") {
				throw std::exception();
			}
		}

		// Last
		{
			utf8string str = u8"ăîșț";
			utf8string_iter str_iter = str.last();
			utf8string new_content = u8"Ă";

			utf8string new_str = str;
			new_str.insertAfter(str_iter, new_content);

			if (new_str != u8"ăîșțĂ") {
				throw std::exception();
			}
		}
	}

	void testPushAndPrepend()
	{
		// Append
		{
			utf8string str = u8"Cârnați";
			str.push(u8" cu mămăliguță");

			if (str != u8"Cârnați cu mămăliguță") {
				throw std::exception();
			}
		}
		
		// Prepend
		{
			utf8string str = u8" cu mămăliguță";
			str.prepend(u8"Cârnați");

			if (str != u8"Cârnați cu mămăliguță") {
				throw std::exception();
			}
		}
	}

	void testForLoop()
	{
		utf8string str = u8"ăĂîșț";

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
			utf8string str = u8"ăĂîșț";
			str.pop(3);

			if (str != u8"ăĂ") {
				throw std::exception();
			}
		}

		// Pop Front
		{
			utf8string str = u8"ăĂîșț";
			str.popFront(3);

			if (str != u8"șț") {
				throw std::exception();
			}
		}
	}

	void testSearch()
	{
		// Contains
		{
			utf8string str = u8"ăĂîșț";

			if (str.contains(u8"îș") == false ||
				str.contains(u8"Ăîș") == false ||
				str.contains(u8"ăĂîșț") == false)
			{
				throw std::exception();
			}
		}

		// find
		{
			utf8string str = u8"ăĂîșț";
			auto iter = str.find(u8"îș");

			if (iter.byte_index != 4 || iter.get() != u8"î") {
				throw std::exception();
			}
		}
	}

	void testEverything()
	{
		testInsertAfter();
		testPushAndPrepend();
		testErase();
		testForLoop();
		testSearch();
	}
}


int main()
{
	Console::configureForUTF8();

	utf8string_tests::testEverything();

	utf8string str = u8"Cârnați cu mămăliguță";
	printf("%s \n", str.c_str());
}
