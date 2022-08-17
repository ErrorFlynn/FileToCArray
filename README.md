## Description

**File To C Array** is a tool that generates a C array from the contents of any file. Such arrays can be used as a way of embedding resource files into an executable binary, gaining access to the resource files without having to use platform-dependent code. The tool requires [Nana C++ Library](https://github.com/cnjinhao/nana) in order to build, but you can download a Windows binary below.

![FileToCArray](https://user-images.githubusercontent.com/20293505/54069428-ea155300-4225-11e9-8288-3d9c7ddbccf9.png)

Protip: if the output file that you choose is the same as the input file, it will automatically get renamed like in the screenshot above.

## Windows binary
https://github.com/ErrorFlynn/FileToCArray/releases/download/v1.1/FileToCArray_11.7z

## Building

### GNU GCC

```
g++ --std=c++17 FileToCArray_main.cpp -lnana -lpng -ljpeg -o FileToCArray
```

#### Building on Windows with MinGW/GCC

If compiling on Windows you will also need to link to some Windows standard libraries:

```
g++ --std=c++17 FileToCArray_main.cpp -lnana -lpng -ljpeg -lgdi32 -lcomdlg32 -mwindows -o FileToCArray
```

To add an icon resource do the following:

```
windres icon_resource.rc -o icon_resource.o

g++ --std=c++17 FileToCArray_main.cpp icon_resource.o -lnana -lpng -ljpeg -lgdi32 -lcomdlg32 -mwindows -o FileToCArray
```
