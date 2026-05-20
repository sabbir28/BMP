# Using the BMP Library from Python

You can easily use `BMP.dll` in Python using the built-in `ctypes` module.

## 1. Prerequisites
- `BMP.dll` must be in the same folder as your Python script or in your system PATH.

## 2. Basic Example

Save this as `main.py`:

```python
import ctypes
from ctypes import Structure, c_uint16, c_uint32, c_int32, c_uint8, POINTER, c_char_p, c_bool

# Define the C-compatible Structures
class BMPColor(Structure):
    _pack_ = 1
    _fields_ = [
        ("blue", c_uint8),
        ("green", c_uint8),
        ("red", c_uint8),
        ("reserved", c_uint8),
    ]

# Load the DLL
bmp_lib = ctypes.CDLL("./BMP.dll")

# Setup function signatures
bmp_lib.BMP_Read.argtypes = [c_char_p]
bmp_lib.BMP_Read.restype = ctypes.c_void_p  # Returns pointer to BMPImage

bmp_lib.BMP_ToGrayscale.argtypes = [ctypes.c_void_p]
bmp_lib.BMP_ToGrayscale.restype = c_bool

bmp_lib.BMP_Write.argtypes = [c_char_p, ctypes.c_void_p]
bmp_lib.BMP_Write.restype = c_bool

bmp_lib.BMP_Destroy.argtypes = [ctypes.c_void_p]

# Usage
path = b"input.bmp"
image_ptr = bmp_lib.BMP_Read(path)

if image_ptr:
    print("Image loaded successfully!")
    if bmp_lib.BMP_ToGrayscale(image_ptr):
        print("Converted to grayscale.")
        bmp_lib.BMP_Write(b"output_python.bmp", image_ptr)
    
    bmp_lib.BMP_Destroy(image_ptr)
else:
    print("Failed to load image.")
```

## 3. Important Notes
- **String Encoding**: Always use `b"string"` (bytes) for filenames when passing them to C functions.
- **Pointer Management**: C is responsible for memory allocation; always call `BMP_Destroy` via `ctypes` to avoid memory leaks in your Python application.
- **Structure Packing**: The C library uses `#pragma pack(1)`, so ensure `_pack_ = 1` is set in your `ctypes.Structure` definitions.
