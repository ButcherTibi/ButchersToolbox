# ButchersToolbox

A C++20 static library of stuff that I use in my other projects.

# Requirements

Visual Studio 2022 with C++20 standard.

# Installation

Just press the `Run` button, by default it will build the library and run the testing project.

The `lib` is found under `ButchersToolbox/Library/x64/Debug`, headers are located in `ButchersToolbox/Source Code/ButchersToolbox`.

# Table of Contents
1. [`utf8_string` library](#utf8_string)
2. [`JSON` parser and writer](#json)
3. [`Console` namespace for text formating](#console)
4. [`Filesys` for reading files and handling filesystem paths](#filesys)

# Content

### [utf8_string](./ButchersToolbox/Source%20Code/ButchersToolbox/utf8_string/)
UTF-8 string library similar to `System.String` from C#.
**This is feature complete.**

### [JSON](./ButchersToolbox/Source%20Code/ButchersToolbox/JSON/)
Methods for parsing, modifying and writing JSON. Complete with pretty indentation of the JSON text and error messages.

### [Console](./ButchersToolbox/Source%20Code/ButchersToolbox/Console/)
Ever wandered how to print to console colored text ?
Did you know you need to configure the console terminal, to enable UTF-8 to work ?

### [Filesys](./ButchersToolbox/Source%20Code/ButchersToolbox/Filesys/)
It simply started with my hate of using regular expressions to handle filesystem paths.
I used this extensively to read compiled HLSL with no extra copies directly into a `std::vector`, also used the Win32 `FILE_FLAG_SEQUENTIAL_SCAN` to make reads faster.
*This is bare bones.*

# TODO

- maybe add DirectX 11 library ?