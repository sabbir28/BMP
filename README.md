# BMP Comprehensive Library (SABBIR28)

A professional C library for reading, writing, and manipulating BMP images.

## Documentation
- [**Detailed Usage and Deployment Guide**](docs/usage.md) - How to use this in your project.
- [**API Reference**](docs/api_reference.md) - Full list of functions and parameters.
- [**Python Integration Guide**](docs/python_integration.md) - How to use this DLL from Python.

### 1. Requirements
- CMake 3.20+
- C11 Compiler (MSVC, GCC, or Clang)

### 2. Including in your Project
Copy the `include/BMP` folder to your project and link against `BMP.dll`.

```c
#include <BMP/BMP.h>

int main() {
    BMPImage* img = BMP_Read("input.bmp");
    if (img) {
        BMP_ToGrayscale(img);
        BMP_Write("output.bmp", img);
        BMP_Destroy(img);
    }
    return 0;
}
```

### 3. API Overview
- `BMP_Create(w, h, bpp)`: Create a new image.
- `BMP_Read(path)`: Load from file.
- `BMP_Write(path, img)`: Save to file.
- `BMP_ToGrayscale(img)`: Convert to grayscale.
- `BMP_PrintHeaderDetails(img)`: Output metadata to console.
- `BMP_SaveHeaderDetails(img, path)`: Save metadata to text file.## Build and Automation
This project uses **GitHub Actions** for automated builds and releases. Every push to the `master` branch triggers a build, and tagged versions (e.g., `v1.0.0`) automatically create a GitHub Release with the bundled library.
