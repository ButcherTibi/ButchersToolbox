// Standard
#include <stdio.h>
#include <array>


// Mine
#include "Tests/utf8_string/utf8_string.hpp"
#include "Tests/JSON/JSON.hpp"
#include "Tests/SparseVector/SparseVector.hpp"

#include "Examples\utf8_string.hpp"

#include "ButchersToolbox\Console\Console.hpp"
#include "ButchersToolbox\Pointer\Pointers.hpp"
#include "ButchersToolbox/Filesys/Filesys.hpp"


namespace Pointer_tests {

	struct TestObject {
		uint32_t member_0;
	};

	void testCorrectness()
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


int main()
{
	// setting utf-8 source code and execution is also required
	// via compiler option '/utf-8'
	Console::configureForUTF8();
	Console::enableVirtualProcessing();

	Console::Style ok_style;
	ok_style.foreground_color = { 0, 255, 0 };

	// Test utf8_string
	{
		utf8_string_tests::testCorrectness();

#if false 
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
#endif		
	}

	JSON_tests::testCorrectness();
	SparseVector_tests::testCorrectness();
	Pointer_tests::testCorrectness();

	Console::log("All correctness tests have passed.", ok_style);

#if false
	JSON_tests::performance();
	Console::log("All performance tests have passed.", ok_style);
#endif

	Console::resetFormatting();
}
