#ifndef BMP_H
#define BMP_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#ifdef _WIN32
    #ifdef BMP_EXPORTS
        #define BMP_API __declspec(dllexport)
    #else
        #define BMP_API __declspec(dllimport)
    #endif
#else
    #define BMP_API
#endif

#pragma pack(push, 1)

typedef struct {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
} BMPFileHeader;

typedef struct {
    uint32_t biSize;
    int32_t  biWidth;
    int32_t  biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t  biXPelsPerMeter;
    int32_t  biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} BMPInfoHeader;

typedef struct {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    uint8_t reserved;
} BMPColor;

#pragma pack(pop)

typedef struct {
    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;
    BMPColor* palette;
    uint8_t* data;
} BMPImage;

// Creation and Destruction
BMP_API BMPImage* BMP_Create(int32_t width, int32_t height, uint16_t bitCount);
BMP_API void BMP_Destroy(BMPImage* image);

// IO Operations
BMP_API BMPImage* BMP_Read(const char* filename);
BMP_API bool BMP_Write(const char* filename, const BMPImage* image);

// Pixel Access (Simplified for 24/32-bit initially, but supports all formats via raw data)
BMP_API bool BMP_SetPixel(BMPImage* image, int32_t x, int32_t y, BMPColor color);
BMP_API bool BMP_GetPixel(const BMPImage* image, int32_t x, int32_t y, BMPColor* color);

// Information
BMP_API int32_t BMP_GetWidth(const BMPImage* image);
BMP_API int32_t BMP_GetHeight(const BMPImage* image);
BMP_API uint16_t BMP_GetBitCount(const BMPImage* image);
BMP_API void BMP_PrintHeaderDetails(const BMPImage* image);

BMP_API BMPFileHeader BMP_GetFileHeader(const BMPImage* image);
BMP_API BMPInfoHeader BMP_GetInfoHeader(const BMPImage* image);

// Image Manipulation
BMP_API bool BMP_ToGrayscale(BMPImage* image);

// Logging
BMP_API bool BMP_SaveHeaderDetails(const BMPImage* image, const char* filename);

#endif // BMP_H
