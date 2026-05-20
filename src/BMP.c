#ifndef BMP_EXPORTS
#define BMP_EXPORTS
#endif
#include "../include/BMP/BMP.h"
#include <stdlib.h>
#include <string.h>

static uint32_t GetRowSize(int32_t width, uint16_t bitCount) {
    return ((width * bitCount + 31) / 32) * 4;
}

static uint32_t GetPaletteSize(uint16_t bitCount) {
    if (bitCount <= 8) {
        return (1 << bitCount);
    }
    return 0;
}

BMP_API BMPImage* BMP_Create(int32_t width, int32_t height, uint16_t bitCount) {
    if (bitCount != 1 && bitCount != 4 && bitCount != 8 && bitCount != 16 && bitCount != 24 && bitCount != 32) {
        return NULL;
    }

    BMPImage* image = (BMPImage*)calloc(1, sizeof(BMPImage));
    if (!image) return NULL;

    image->infoHeader.biSize = sizeof(BMPInfoHeader);
    image->infoHeader.biWidth = width;
    image->infoHeader.biHeight = height;
    image->infoHeader.biPlanes = 1;
    image->infoHeader.biBitCount = bitCount;
    image->infoHeader.biCompression = 0; // BI_RGB
    
    uint32_t rowSize = GetRowSize(width, bitCount);
    image->infoHeader.biSizeImage = rowSize * (height > 0 ? height : -height);
    
    uint32_t paletteSize = GetPaletteSize(bitCount);
    if (paletteSize > 0) {
        image->palette = (BMPColor*)calloc(paletteSize, sizeof(BMPColor));
        image->infoHeader.biClrUsed = paletteSize;
    }

    image->fileHeader.bfType = 0x4D42; // "BM"
    image->fileHeader.bfOffBits = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + (paletteSize * sizeof(BMPColor));
    image->fileHeader.bfSize = image->fileHeader.bfOffBits + image->infoHeader.biSizeImage;

    image->data = (uint8_t*)malloc(image->infoHeader.biSizeImage);
    if (!image->data) {
        BMP_Destroy(image);
        return NULL;
    }
    memset(image->data, 0, image->infoHeader.biSizeImage);

    return image;
}

BMP_API void BMP_Destroy(BMPImage* image) {
    if (image) {
        if (image->palette) free(image->palette);
        if (image->data) free(image->data);
        free(image);
    }
}

BMP_API BMPImage* BMP_Read(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) return NULL;

    BMPImage* image = (BMPImage*)calloc(1, sizeof(BMPImage));
    if (!image) {
        fclose(file);
        return NULL;
    }

    if (fread(&image->fileHeader, sizeof(BMPFileHeader), 1, file) != 1) goto error;
    if (image->fileHeader.bfType != 0x4D42) goto error;

    if (fread(&image->infoHeader, sizeof(BMPInfoHeader), 1, file) != 1) goto error;

    uint32_t paletteSize = GetPaletteSize(image->infoHeader.biBitCount);
    if (paletteSize > 0) {
        image->palette = (BMPColor*)calloc(paletteSize, sizeof(BMPColor));
        if (!image->palette) goto error;
        fread(image->palette, sizeof(BMPColor), paletteSize, file);
    }

    fseek(file, image->fileHeader.bfOffBits, SEEK_SET);
    
    uint32_t size = image->infoHeader.biSizeImage;
    if (size == 0) {
        size = GetRowSize(image->infoHeader.biWidth, image->infoHeader.biBitCount) * (image->infoHeader.biHeight > 0 ? image->infoHeader.biHeight : -image->infoHeader.biHeight);
    }
    
    image->data = (uint8_t*)malloc(size);
    if (!image->data) goto error;
    if (fread(image->data, 1, size, file) != size) goto error;

    fclose(file);
    return image;

error:
    if (file) fclose(file);
    BMP_Destroy(image);
    return NULL;
}

BMP_API bool BMP_Write(const char* filename, const BMPImage* image) {
    if (!image) return false;
    FILE* file = fopen(filename, "wb");
    if (!file) return false;

    fwrite(&image->fileHeader, sizeof(BMPFileHeader), 1, file);
    fwrite(&image->infoHeader, sizeof(BMPInfoHeader), 1, file);

    uint32_t paletteSize = GetPaletteSize(image->infoHeader.biBitCount);
    if (paletteSize > 0 && image->palette) {
        fwrite(image->palette, sizeof(BMPColor), paletteSize, file);
    }

    fseek(file, image->fileHeader.bfOffBits, SEEK_SET);
    fwrite(image->data, 1, image->infoHeader.biSizeImage, file);

    fclose(file);
    return true;
}

BMP_API bool BMP_SetPixel(BMPImage* image, int32_t x, int32_t y, BMPColor color) {
    if (!image || x < 0 || x >= image->infoHeader.biWidth || y < 0 || y >= (image->infoHeader.biHeight > 0 ? image->infoHeader.biHeight : -image->infoHeader.biHeight)) return false;

    uint32_t rowSize = GetRowSize(image->infoHeader.biWidth, image->infoHeader.biBitCount);
    uint8_t* row = image->data + (y * rowSize);

    if (image->infoHeader.biBitCount == 24) {
        row[x * 3 + 0] = color.blue;
        row[x * 3 + 1] = color.green;
        row[x * 3 + 2] = color.red;
    } else if (image->infoHeader.biBitCount == 32) {
        row[x * 4 + 0] = color.blue;
        row[x * 4 + 1] = color.green;
        row[x * 4 + 2] = color.red;
        row[x * 4 + 3] = color.reserved;
    } else if (image->infoHeader.biBitCount == 8) {
        // For 8-bit, we treat color.blue as the palette index for simplicity in this helper
        row[x] = color.blue;
    }
    // Other bit depths (1, 4, 16) require bit manipulation which can be added if needed
    
    return true;
}

BMP_API bool BMP_GetPixel(const BMPImage* image, int32_t x, int32_t y, BMPColor* color) {
    if (!image || !color || x < 0 || x >= image->infoHeader.biWidth || y < 0 || y >= (image->infoHeader.biHeight > 0 ? image->infoHeader.biHeight : -image->infoHeader.biHeight)) return false;

    uint32_t rowSize = GetRowSize(image->infoHeader.biWidth, image->infoHeader.biBitCount);
    const uint8_t* row = image->data + (y * rowSize);

    if (image->infoHeader.biBitCount == 24) {
        color->blue = row[x * 3 + 0];
        color->green = row[x * 3 + 1];
        color->red = row[x * 3 + 2];
        color->reserved = 0;
    } else if (image->infoHeader.biBitCount == 32) {
        color->blue = row[x * 4 + 0];
        color->green = row[x * 4 + 1];
        color->red = row[x * 4 + 2];
        color->reserved = row[x * 4 + 3];
    } else if (image->infoHeader.biBitCount == 8) {
        uint8_t index = row[x];
        if (image->palette) {
            *color = image->palette[index];
        } else {
            color->red = color->green = color->blue = index; // Grayscale fallback
            color->reserved = 0;
        }
    }
    
    return true;
}

BMP_API int32_t BMP_GetWidth(const BMPImage* image) { return image ? image->infoHeader.biWidth : 0; }
BMP_API int32_t BMP_GetHeight(const BMPImage* image) { return image ? image->infoHeader.biHeight : 0; }
BMP_API uint16_t BMP_GetBitCount(const BMPImage* image) { return image ? image->infoHeader.biBitCount : 0; }

BMP_API void BMP_PrintHeaderDetails(const BMPImage* image) {
    if (!image) return;
    
    printf("--- BMP File Header ---\n");
    printf("Type: %04X (%c%c)\n", image->fileHeader.bfType, (char)(image->fileHeader.bfType & 0xFF), (char)(image->fileHeader.bfType >> 8));
    printf("Size: %u bytes\n", image->fileHeader.bfSize);
    printf("Offset to Data: %u\n", image->fileHeader.bfOffBits);
    
    printf("\n--- BMP Info Header ---\n");
    printf("Size: %u\n", image->infoHeader.biSize);
    printf("Width: %d\n", image->infoHeader.biWidth);
    printf("Height: %d\n", image->infoHeader.biHeight);
    printf("Planes: %u\n", image->infoHeader.biPlanes);
    printf("Bit Count: %u\n", image->infoHeader.biBitCount);
    printf("Compression: %u\n", image->infoHeader.biCompression);
    printf("Image Size: %u\n", image->infoHeader.biSizeImage);
    printf("X Pels Per Meter: %d\n", image->infoHeader.biXPelsPerMeter);
    printf("Y Pels Per Meter: %d\n", image->infoHeader.biYPelsPerMeter);
    printf("Colors Used: %u\n", image->infoHeader.biClrUsed);
    printf("Colors Important: %u\n", image->infoHeader.biClrImportant);
}

BMP_API BMPFileHeader BMP_GetFileHeader(const BMPImage* image) {
    BMPFileHeader h = {0};
    if (image) h = image->fileHeader;
    return h;
}

BMP_API BMPInfoHeader BMP_GetInfoHeader(const BMPImage* image) {
    BMPInfoHeader h = {0};
    if (image) h = image->infoHeader;
    return h;
}

BMP_API bool BMP_ToGrayscale(BMPImage* image) {
    if (!image) return false;
    
    // Only support 24-bit and 32-bit for simple conversion
    if (image->infoHeader.biBitCount != 24 && image->infoHeader.biBitCount != 32) return false;

    int32_t height = image->infoHeader.biHeight > 0 ? image->infoHeader.biHeight : -image->infoHeader.biHeight;
    int32_t width = image->infoHeader.biWidth;
    uint32_t rowSize = GetRowSize(width, image->infoHeader.biBitCount);
    uint32_t bpp = image->infoHeader.biBitCount / 8;

    for (int32_t y = 0; y < height; y++) {
        uint8_t* row = image->data + (y * rowSize);
        for (int32_t x = 0; x < width; x++) {
            uint8_t* pixel = row + (x * bpp);
            // Luma formula: 0.299R + 0.587G + 0.114B
            uint8_t gray = (uint8_t)(0.299f * pixel[2] + 0.587f * pixel[1] + 0.114f * pixel[0]);
            pixel[0] = pixel[1] = pixel[2] = gray;
        }
    }
    return true;
}

BMP_API bool BMP_SaveHeaderDetails(const BMPImage* image, const char* filename) {
    if (!image || !filename) return false;
    FILE* f = fopen(filename, "w");
    if (!f) return false;

    fprintf(f, "--- BMP File Header ---\n");
    fprintf(f, "Type: %04X\n", image->fileHeader.bfType);
    fprintf(f, "Size: %u bytes\n", image->fileHeader.bfSize);
    fprintf(f, "Offset to Data: %u\n", image->fileHeader.bfOffBits);
    
    fprintf(f, "\n--- BMP Info Header ---\n");
    fprintf(f, "Size: %u\n", image->infoHeader.biSize);
    fprintf(f, "Width: %d\n", image->infoHeader.biWidth);
    fprintf(f, "Height: %d\n", image->infoHeader.biHeight);
    fprintf(f, "Planes: %u\n", image->infoHeader.biPlanes);
    fprintf(f, "Bit Count: %u\n", image->infoHeader.biBitCount);
    fprintf(f, "Compression: %u\n", image->infoHeader.biCompression);
    fprintf(f, "Image Size: %u\n", image->infoHeader.biSizeImage);
    fprintf(f, "X Pels Per Meter: %d\n", image->infoHeader.biXPelsPerMeter);
    fprintf(f, "Y Pels Per Meter: %d\n", image->infoHeader.biYPelsPerMeter);
    fprintf(f, "Colors Used: %u\n", image->infoHeader.biClrUsed);
    fprintf(f, "Colors Important: %u\n", image->infoHeader.biClrImportant);

    fclose(f);
    return true;
}
