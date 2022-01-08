## ButchersToolbox
A collection of stuff that I commonly use in my projects.

### Table of Contents
1. `utf8_string` library
2. `Console` namespace for text formating
3. `Filesys` for reading files and handling filesystem paths

### Requirements
Visual Studio 2022 with C++ 20 standard.

### utf8_string library
One day I got really annoyed about the fact that C++ does not have a nice and easy to use unicode string library like C#.
It was easier than I thought.
**This thing is getting good**

### Console
Ever wandered how to print to console colored text ?
Did you know you need to configure the console terminal, to enable UTF-8 to work ?

### Filesys
It simply started with my hate of using regular expressions to handle filesystem paths.
I used this extensively to read compiled HLSL with no extra copies directly into a `std::vector`, also used the Win32 `FILE_FLAG_SEQUENTIAL_SCAN` to make read faster.
*Needs more work*
