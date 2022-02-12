## ButchersToolbox
A toolbox of stuff that I use in my projects.

### Table of Contents
1. [`utf8_string` library](#utf8_string)
2. [`Console` namespace for text formating](#console)
3. [`Filesys` for reading files and handling filesystem paths](#filesys)

### Requirements
Visual Studio 2022 with C++ 20 standard.

### utf8_string
A C++20 utf-8 string library similar to `System.String` from C#.
**Almost feature complete**.
[link](./ButchersToolbox/Source Code/ButchersToolbox/utf8_string/README.md)

### Console
Ever wandered how to print to console colored text ?
Did you know you need to configure the console terminal, to enable UTF-8 to work ?

### Filesys
It simply started with my hate of using regular expressions to handle filesystem paths.
I used this extensively to read compiled HLSL with no extra copies directly into a `std::vector`, also used the Win32 `FILE_FLAG_SEQUENTIAL_SCAN` to make read faster.
*Barely started*
