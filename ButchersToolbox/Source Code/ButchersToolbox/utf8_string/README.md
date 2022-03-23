## UTF-8 C++20 String Library

This is a simple, easy to use library for common string operations inspired by C# `System.String` class.

> **WARNING**: To enable proper UTF-8 support for C++ programs in Visual Studio you must configure the console output to [utf-8](https://docs.microsoft.com/en-us/windows/console/setconsoleoutputcp) as well compiling with the [`/utf-8`](https://docs.microsoft.com/en-us/cpp/build/reference/utf-8-set-source-and-executable-character-sets-to-utf-8?view=msvc-170) flag.
> Failure to set the `/utf-8` will cause the 'ă' character to be displayed as 'a' and a warning to be emited in regards to the codepage of the source file.

### Example 1 Creating a string
```C++
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
```

### Example 2 Accessing a character
```C++
utf8_string str = u8"Cârnați";

// access character
printf("%s %s %s \n",
    str[0].c_str(), str[1].c_str(), str[6].c_str()
);

// access by byte index
printf("%s %s %s \n",
    str.at(0).c_str(), str.at(1).c_str(), str.at(8).c_str()
);
```

### Example 3 Comparison
```C++
utf8_string soup = u8"Supă";
utf8_string mayo = u8"Maioneză";

if (soup != mayo) {
    printf("strings are not equal \n");
}
```

### Example 4 Find
```C++
utf8_string str = u8"Mămăliga se face cu mălai";
utf8_string_iter found = str.find(u8"face");
printf("word 'face' was found at %d \n", found.characterIndex());

printf("Does string start with 'Mămăliga' = %d \n", str.startsWith(u8"Mămăliga"));

printf("Does string end with 'mălai' = %d \n", str.endsWith(u8"mălai"));
```

### Example 5 Modify
```C++
utf8_string str = u8"Ceafă de porc";
str.push(u8" şi cartofi cu rozmarin");
printf("%s \n", str.c_str());

utf8_string_iter insert_location = str.begin();
insert_location.next(12);
str.insert(insert_location, u8" la grătar");
printf("%s \n", str.c_str());

str.erase(u8" cu rozmarin");
printf("%s \n", str.c_str());
```

### Example 6 Iteration
```C++
utf8_string str = u8"La Vatra au niște șnițele bune\n";

for (auto iter = str.begin(); iter != str.after(); iter++) {
    printf("%s", iter.get().c_str());  // not very efficient
}

for (auto iter = str.last(); iter != str.before(); iter--) {
    printf("%s", iter.get().c_str());  // not very efficient
}
```
