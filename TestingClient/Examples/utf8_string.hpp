﻿#pragma once

#include "ButchersToolbox\utf8_string\utf8_string.hpp"


namespace utf8string_examples {

	void example_1_Creating_a_string()
	{
		utf8_string str = u8"This is a UTF-8 encoded string";
		printf("%s \n", str.c_str());

		std::string std_str = "utf8_string form standard library string";
		utf8_string str_from_std = std_str;
		printf("%s \n", str_from_std.c_str());

		std::vector<uint8_t> bytes = {
			'B', 'y', 't', 'e', 's'
		};
		utf8_string str_bytes = bytes;	
		printf("%s \n", str_bytes.c_str());
	}

	void example_2_Accessing_a_character()
	{
		utf8_string str = u8"Cârnați";

		// access character
		printf("%s %s %s \n",
			str[0].c_str(), str[1].c_str(), str[6].c_str()
		);

		// access by byte index
		printf("%s %s %s \n",
			str.at(0).c_str(), str.at(1).c_str(), str.at(8).c_str()
		);
	}

	void example_3_Comparison()
	{
		utf8_string soup = u8"Supă";
		utf8_string mayo = u8"Maioneză";

		if (soup != mayo) {
			printf("strings are not equal \n");
		}
	}

	void example_4_Find()
	{
		utf8_string str = u8"Mămăliga se face cu mălai";
		utf8_string_iter found = str.find(u8"face");
		printf("word 'face' was found at %d \n", found.characterIndex());

		printf("Does string start with 'Mămăliga' = %d \n", str.startsWith(u8"Mămăliga"));

		printf("Does string end with 'mălai' = %d \n", str.endsWith(u8"mălai"));
	}

	void example_5_Modify()
	{
		utf8_string str = u8"Ceafă de porc";
		str.push(u8" şi cartofi cu rozmarin");
		printf("%s \n", str.c_str());

		utf8_string_iter insert_location = str.begin();
		insert_location.next(12);
		str.insert(insert_location, u8" la grătar");
		printf("%s \n", str.c_str());

		str.erase(u8" cu rozmarin");
		printf("%s \n", str.c_str());
	}

	void example_6_Iteration()
	{
		utf8_string str = u8"La Vatra au niște șnițele bune\n";
		
		for (auto iter = str.begin(); iter != str.after(); iter++) {
			
			printf("%s", iter.get().c_str());  // not very efficient
		}

		for (auto iter = str.last(); iter != str.before(); iter--) {

			printf("%s", iter.get().c_str());  // not very efficient
		}

		printf("\n");
	}
}
