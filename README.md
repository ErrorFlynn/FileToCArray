
**File To C Array** is a tool that generates a C array from the contents of any file. Such arrays can be used as a way of embedding resource files into an executable file.
The GUI is Windows-only and requires [Nana C++ GUI Library](https://github.com/cnjinhao/nana) to build, but you can download a binary here:

https://github.com/ErrorFlynn/FileToCArray/releases/download/v2.0/FileToCArray.7z


<img width="602" height="257" alt="File To C Array GUI" src="https://github.com/user-attachments/assets/8544fbd5-4fb7-43a0-b8d3-0d0cf066e1b4" />

## File To C Array 2.0

After years of neglect, I have finally turned my attention back to this project, and have given it a much needed refresh. Here are the changes from v1.1:

### Much improved GUI and operation

- GUI scales with system scale factor
- GUI follows system color theme (dark/light)
- window accepts files dragged and dropped onto it
- multiple files can be queued up

### The GUI code is now decoupled from the array-generating code

The code that actually generates C arrays now resides in its own class that you can use in your own project. It's completely separate from the Windows-only GUI, and should be platform-independent as far as I can tell.

Basic example:

```cpp
#include "file_to_c_array.hpp"

int main()
{
	file_to_c_array ftca;
	ftca.generate("c:\\myfile.ext", "c:\\arr_myfile_ext.hpp");
}
```

The code will try to minimize the size of the output file by not using a space after commas, by using the widest possible array elements (the wider the element, the less commas used), and by using hex numbers when it makes sense. If the output file already exists, the array is appended to it. The code will throw a std::exception if an error occurs.

### Added a license

I have now explicitly put the project in the public domain (with the exception of the file `subclass.hpp` which has the Boost license), which had been my intention from the beginning. I hadn't bothered to add a license file, not realizing how restrictive that is (the opposite of my intention).
