# BMP Library API Reference

## Image Lifecycle

### `BMPImage* BMP_Create(int32_t width, int32_t height, uint16_t bitCount)`
Creates a new BMP image in memory.
- **width/height**: Dimensions of the image.
- **bitCount**: Depth (1, 4, 8, 16, 24, or 32).
- **Returns**: A pointer to `BMPImage`, or `NULL` on failure.

### `void BMP_Destroy(BMPImage* image)`
Frees all memory associated with the image.
- **image**: The pointer returned by `BMP_Create` or `BMP_Read`.

## File IO

### `BMPImage* BMP_Read(const char* filename)`
Loads a BMP file from disk.
- **filename**: Complete path to the `.bmp` file.

### `bool BMP_Write(const char* filename, const BMPImage* image)`
Saves the image to disk.
- **Returns**: `true` if success, `false` otherwise.

## Manipulation

### `bool BMP_ToGrayscale(BMPImage* image)`
Converts a 24-bit or 32-bit image to grayscale in-place.
- **Note**: Currently only supports direct color images (24/32-bit).

### `bool BMP_SetPixel(BMPImage* image, int32_t x, int32_t y, BMPColor color)`
Sets a specific pixel color.
- **For 8-bit**: Uses `color.blue` as the palette index.

## Information and Logging

### `void BMP_PrintHeaderDetails(const BMPImage* image)`
Prints file and info header metadata to standard output (console).

### `bool BMP_SaveHeaderDetails(const BMPImage* image, const char* filename)`
Writes header metadata to a `.txt` file.
- **filename**: Path to the text file to create.
