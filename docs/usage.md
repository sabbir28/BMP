# Developer Integration and Deployment Guide

This guide explains how to properly include the SABBIR28 BMP Library in your own C/C++ projects and how to distribute it with your software.

## 1. Getting the Library
You can obtain the library in two ways:
- **Download the Release**: Go to the GitHub repository's "Releases" section and download the `BMP_Library_vX.Y.Z.zip`. This contains the pre-compiled `.dll`, `.lib`, and headers.
- **Build from Source**: Clone the repository and run CMake to build it yourself.

## 2. Integrating into Your Project (Visual Studio / CMake)

### Including Headers
Add the `include` folder to your project's include path. In your code:
```c
#include <BMP/BMP.h>
```

### Linking the Library
- **For Build**: Link your executable against `BMP.lib` (the import library).
- **In CMake**: 
  ```cmake
  target_link_libraries(MyApplication PRIVATE BMP)
  ```

## 3. Distributing the DLL (The "Copy" Rule)
The most common mistake developers make is forgetting to include the DLL with their final EXE.

### Manual Release
When you build your project into a `Release` or `dist` folder, you **MUST** copy `BMP.dll` from the library folder into the **same directory** where your `.exe` resides. Without this, your program will fail to start with a "DLL Not Found" error.

### Automated Release (CMake)
You can automate this copy step in your own CMakeLists.txt so you never forget:
```cmake
add_custom_command(TARGET MyProject POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${BMP_LIBRARY_PATH}/BMP.dll"
    $<TARGET_FILE_DIR:MyProject>)
```

## 4. Best Practices
- Always call `BMP_Destroy(image)` when you are done with an image to prevent memory leaks.
- Check return values (e.g., if `BMP_Read` returns `NULL`, the file was likely missing or corrupt).
