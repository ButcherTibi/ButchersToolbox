// Standard
#include <stdio.h>
#include <array>


// Mine
#include "ButchersToolbox\Console\Console.hpp"
#include "ButchersToolbox\utf8_string\utf8_string.hpp"
#include "ButchersToolbox\Pointer\Pointers.hpp"
#include "ButchersToolbox/SparseVector.hpp"
#include "ButchersToolbox/Filesys/Filesys.hpp"

#include "Examples\utf8_string.hpp"

#include "Tests/JSON.hpp"


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
}


namespace Filesys_tests {

	void test()
	{

	}
}


namespace Pointer_tests {

	struct TestObject {
		uint32_t member_0;
	};

	void test()
	{
		std::vector<TestObject> vec;
		TestObject& obj = vec.emplace_back();
		obj.member_0 = 1;

		vector_ptr ptr(vec, 0);
		ptr->member_0 = 2;

		if (ptr->member_0 != 2) {
			throw std::exception();
		}
	}
}


namespace DefferedVector_tests {

	void test()
	{
		SparseVector<uint32_t> vec;
		vec.resize(4);
		vec[0] = 0;
		vec[1] = 1;
		vec[2] = 2;
		vec[3] = 3;

		for (auto i = vec.begin(); i != vec.after(); i++) {		
			printf("%d, ", i.get());
		}
		printf("\n");

		vec.erase(2);

		for (auto i = vec.begin(); i != vec.after(); i++) {
			printf("%d, ", i.get());
		}
		printf("\n");
	}
}


int main()
{
	// Test utf8_string
	{
		// setting utf-8 source code and execution is also required
		// via compiler option '/utf-8'
		Console::configureForUTF8();

		printf("\nExample 1 Creating a string \n");
		utf8string_examples::example_1_Creating_a_string();
		printf("\nExample 2 Acessing a character \n");
		utf8string_examples::example_2_Accessing_a_character();
		printf("\nExample 3 Comparison \n");
		utf8string_examples::example_3_Comparison();
		printf("\nExample 4 Find \n");
		utf8string_examples::example_4_Find();
		printf("\nExample 5 Modify \n");
		utf8string_examples::example_5_Modify();
		printf("\nExample 6 Iteration \n");
		utf8string_examples::example_6_Iteration();

		utf8_string_tests::testInsertAfter();
		utf8_string_tests::testPushAndPrepend();
		utf8_string_tests::testErase();
		utf8_string_tests::testForLoop();
		utf8_string_tests::testSearch();
		utf8_string_tests::testJoin();
	}

	// Filesys
	{
		// Filesys_tests::test();
	}

	// Pointers
	{
		Pointer_tests::test();
	}

	// Deffered Vector
	{
		DefferedVector_tests::test();
	}

	// JSON
	{
		// JSON_tests::correctness();
		// JSON_tests::performance();
	}

	printf("\nTests have been run \n");
}
